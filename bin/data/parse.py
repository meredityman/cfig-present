import csv
from jinja2 import Template
from pathlib import Path



template_string = """#pragma once
#include "ofMain.h"

class Location {
public:
    {% for col in header -%}
    std::string {{ col }};
    {% endfor -%}
    
    Location({{ constructor_args }}) :
        {% for col in header[:-1] -%}
        {{ col }}({{ col }}),
        {% endfor -%}
        {{ header[-1] }}({{ header[-1] }})
    { }
};

std::vector<Location> locations = {
    {% for row in rows -%}
    Location({{ row }}),
    {% endfor -%}
};
"""

filename = Path("/media/meredityman/data-ssd/data/work/production/cfig/Code/cfig-present/bin/data/locations.csv")
locations_h = Path("/media/meredityman/data-ssd/data/work/production/cfig/Code/cfig-present/src/location.h")


template = Template(template_string)

with open(filename, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    header = reader.fieldnames
    safe_header = [ s.replace("-", "_") for s in header]

    constructor_args = ", ".join([f"const std::string& {col}" for col in safe_header])
    rows = []
    for row in reader:
        init_row = ", ".join([f'"{row[col]}"' for col in header])

        init_row = init_row.replace("\n", "<br>")
        rows.append(init_row)
        print(init_row )

    
    rendered_template = template.render(header=safe_header, constructor_args=constructor_args, rows=rows)

with open(locations_h , "w") as output_file:
    output_file.write(rendered_template)
