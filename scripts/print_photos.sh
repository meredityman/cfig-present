#!/bin/bash

lpoptions -p Canon_TS9100_series -l

lp -d Canon_TS9100_series -o media=4x6.Borderless \
    -o MediaType=Photographic \
    -o PageSize=4x6.Borderless \
    -o page-ranges=1 \
    photos.pdf 