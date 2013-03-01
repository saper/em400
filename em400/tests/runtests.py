#!/usr/bin/python

#  Copyright (c) 2013 Jakub Filipowicz <jakubf@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

import os
import sys
import re
import subprocess

assem = "../../assem/assem"
em400 = "../build/em400"


# ------------------------------------------------------------------------
def run_assem(source):
    output = "out.bin"
    args = [assem, source, output]
    try:
        o = subprocess.check_output(args)
    except:
        return None
    return output

# ------------------------------------------------------------------------
def get_test_data(source):
    xpcts = []
    program_name = ""

    for l in open(source):

        # get program name
        pname = re.findall(".program[ \t]+\"(.*)\"", l)
        if pname:
            program_name = pname[0]

        # get test result conditions
        xpct = re.findall(";[ \t]*XPCT[ \t]+([^ \t:]+)[ \t]*:[ \t]*(.*)", l)
        if len(xpct) == 0 or len(xpct[0]) != 2:
            continue
        else:
            xpcts.append(xpct[0])

    return program_name, xpcts

# ------------------------------------------------------------------------
def compose_test(xpcts):
    q = ', '.join([x[0] for x in xpcts])
    a = ' '.join([x[1] for x in xpcts])
    return q, a

# ------------------------------------------------------------------------
def run_em400(program, q, a):
    args = [em400, "-p", program, "-t", q]
    try:
        o = subprocess.check_output(args)
    except Exception, e:
        return None

    tres = re.findall("TEST RESULT: (.*) \n", o)
    if not tres:
        return None

    return tres[0]

# ------------------------------------------------------------------------
# --- MAIN ---------------------------------------------------------------
# ------------------------------------------------------------------------

if len(sys.argv) != 1:
    tests = sys.argv[1:]
else:
    tests = []
    for path, dirs, files in os.walk("."):
        for f in files:
            if f.endswith(".asm"):
                tests.append(os.path.join(path, f))
    tests.sort()

# run tests

for t in tests:
    binary = run_assem(t)
    result = "?"

    if binary is not None:
        program_name, xpcts = get_test_data(t)
        if xpcts is not None:
            if not program_name:
                program_name = t

            q, a = compose_test(xpcts)

            result = run_em400(binary, q, a)

            if result is None:
                result = "FAIL: em400"
            else:
                if result != a:
                    result = "FAIL: %s" % result
                else:
                    result = "PASS"

        else:
            result = "FAIL: xpcts"
    else:
        result = "FAIL: assem"

    print "Test: %-30s -> %s" % (program_name, result)


# vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4