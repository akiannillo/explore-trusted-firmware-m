# -*- coding: utf-8 -*-
#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#------------------------------------------------------------------------------#

# Interface module for alligning project's Sphinx Compatibility with
# readthedocs service

#
# This file is mimicing the behavior of tf-m/cmake/SphinxCopyDoc.cmake
# which when triggered by CMAKE collects all documents in a intermediate
# location before calling sphinx-build.

# It can be triggered by simply importing the module will should produce a
# an identical output, while retaining standard Sphinx behavior.


import os
import re
from shutil import copy2
from glob import glob

# Determine absolute paths for tf-m project and current directory
read_the_doc_root = os.path.dirname(os.path.abspath(__file__))
tfm_root = os.path.dirname(read_the_doc_root)
doc_root = os.path.join(tfm_root, "docs")

doc_files = []

# Recursively list all files with extensions and add them
for ext in [".rst", ".md", ".txt", ".png", ".jpg"]:
    doc_files.extend([f for f in glob(os.path.join(tfm_root, "**/*%s" % ext),
                      recursive=True)])

# Do not add files from this folder
doc_files = filter(lambda x: read_the_doc_root not in x, doc_files)

for df in list(doc_files):

    # Set the target filename to be cwd + relative to root path of origin
    target_f = df.replace(tfm_root, "").lstrip("/")
    target_f = os.path.join(os.getcwd(), target_f)
    # Create path for file (nested) without exception if exists
    os.makedirs(os.path.dirname(target_f), exist_ok=True)

    # Copy the file to new location
    print("Copying %s %s -> %s" % (df, " " * (90 - len(df)), target_f))
    copy2(df, target_f)

index_f_origin = os.path.join(doc_root, "index.rst.in")
index_f = os.path.join(read_the_doc_root, "index.rst")


# Copy the index from docs directory and strip the CMAKE variable references
copy2(index_f_origin, index_f)

with open(index_f, "r") as F:
    index_data = F.read()
    index_data = re.sub(r'@[A-Z\_]+@', "", index_data)

with open(index_f, "w") as F:
    F.write(index_data)
