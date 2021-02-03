#!/bin/bash

# this is typically built by the CI only.
# For users, ktool will pull it from ghcr.io directly.
docker build -t ghcr.io/jschnurr/cosmos-buildenv:v1 .