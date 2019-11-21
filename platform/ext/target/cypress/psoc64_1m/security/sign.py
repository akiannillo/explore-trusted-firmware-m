#!/usr/bin/python3
"""
Copyright (c) 2019 Cypress Semiconductor Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import cysecuretools
import sys, getopt
import os
from shutil import copyfile, move

def main(argv):
    s_hex_file=""
    ns_hex_file=""
    policy_file=""
    try:
        opts, args = getopt.getopt(argv,"hs:n:p:", ["s_hex=", "ns_hex=", "policy="])
    except getopt.GetoptError:
        print('sign.py [-s <tfm_s hex>] [-n <tfm_ns hex>] -p <policy json>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('sign.py [-s <tfm_s hex>] [-n <tfm_ns hex>] -p <policy json>')
            sys.exit()
        elif opt in ("-s", "--s_hex"):
            s_hex_file = arg
        elif opt in ("-n", "--ns_hex"):
            ns_hex_file = arg
        elif opt in ("-p", "--policy"):
            policy_file = arg

    if not s_hex_file and not ns_hex_file:
        print('Error: no files to sign')
        exit(1)

    if not policy_file:
        print('Error: missing policy file, specify it with -policy <policy json>')
        exit(1)
    else:
        print('policy file:', policy_file)

    if s_hex_file:
        print('signing tfm_s image:', s_hex_file)

        # sign_image overwrites original image, make a copy of it
        name, ext = os.path.splitext(s_hex_file)
        s_hex_signed_file = name + '_signed' + ext
        try:
            copyfile(s_hex_file, s_hex_signed_file)
        except IOError as e:
            print("Failed to copy file '{}' to '{}' ({})"
                   .format(s_hex_file, s_hex_signed_file, e.message))
            raise

        cysecuretools.sign_image(s_hex_signed_file, policy_file, 1)

    if ns_hex_file:
        print('signing tfm_ns image:', ns_hex_file)

        name, ext = os.path.splitext(ns_hex_file)
        ns_hex_signed_file = name + '_signed' + ext
        try:
            copyfile(ns_hex_file, ns_hex_signed_file)
        except IOError as e:
            print("Failed to copy file '{}' to '{}' ({})"
                   .format(ns_hex_file, ns_hex_signed_file, e.message))
            raise

        cysecuretools.sign_image(ns_hex_signed_file, policy_file, 16)

        # for CM4, sign_image creates an unsigned copy of the image
        # named <image to sign>_cm4.hex. Delete it to avoid confusion.
        file_name = name + '_signed_cm4' + ext
        if os.path.isfile(file_name):
            try:
                os.remove(file_name)
            except IOError:
                print("Could not erase '{}'"
                          .format(file_name))

    print('Done.')

if __name__ == "__main__":
   main(sys.argv[1:])
