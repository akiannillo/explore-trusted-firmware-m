#-------------------------------------------------------------------------------
# Copyright (c) 2018-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

import os
import io
from jinja2 import Environment, BaseLoader, select_autoescape

try:
    import yaml
except ImportError as e:
    print (str(e) + " To install it, type:")
    print ("pip install PyYAML")
    exit(1)

class TemplateLoader(BaseLoader):
    """
    Template loader class.

    An instance of this class is passed to the template engine. It is
    responsible for reading the template file
    """
    def __init__(self):
        pass

    def get_source(self, environment, template):
        """
        This function reads the template files.
        For detailed documentation see:
        http://jinja.pocoo.org/docs/2.10/api/#jinja2.BaseLoader.get_source

        Please note that this function always return 'false' as 'uptodate'
        value, so the output file will always be generated.
        """
        if not os.path.isfile(template):
            raise TemplateNotFound(template)
        with open(template) as f:
            source = f.read()
        return source, template, False

def load_manifest_list(file):
    """
    Load the substitution data from the manifests.

    Parameters
    ----------
    file : file
        A yaml file containing the manifest list

    Returns
    -------
    list
        The list of the contents of the manifest files, as generated by the yaml
        parser.
        """
    db = []
    manifest_list = yaml.load(file)
    for item in manifest_list["manifest_list"]:
        manifest_path = item['manifest']
        try:
            file = open(manifest_path)
            manifest = yaml.load(file)
            db.append({"manifest": manifest, "attr": item})
        except IOError:
            raise Exception ("Manifest for "+item['name']+" cannot be opened at path "+item['manifest'])

    return db


def main():
    """
    The entry point of the script.

    Generates the output files based on the templates and the manifests.
    """
    donotedit_warning = \
                    "/*********** " + \
                    "WARNING: This is an auto-generated file. Do not edit!" + \
                    " ***********/"

    env = Environment(
        loader = TemplateLoader(),
        autoescape = select_autoescape(['html', 'xml']),
        lstrip_blocks = True,
        trim_blocks = True,
        keep_trailing_newline = True
    )

    with open(os.path.join('tools', 'tfm_manifest_list.yaml')) \
                                                    as manifest_list_yaml_file:
        # Read manifest list file, build database
        db = load_manifest_list(manifest_list_yaml_file)

        utilities = {}
        utilities['donotedit_warning']=donotedit_warning

        context = {}
        context['manifests'] = db
        context['utilities'] = utilities

    with open(os.path.join('tools', 'tfm_generated_file_list.yaml')) \
                                                    as file_list_yaml_file:
        # read list of files that need to be generated from templates using db
        file_list_yaml = yaml.load(file_list_yaml_file)
        file_list = file_list_yaml["file_list"]
        for file in file_list:
            outfile_name = file["output"]
            templatefile_name = outfile_name + '.template'

            print ("Generating " + outfile_name)

            template = env.get_template(templatefile_name)

            outfile = io.open(outfile_name, "w", newline='\n')
            outfile.write(template.render(context))
            outfile.close()
    print ("Generation of files done")

if __name__ == "__main__":
    main()
