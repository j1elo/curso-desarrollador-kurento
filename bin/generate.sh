#!/usr/bin/env bash

#/ Generate final documents from the existing Markdown files.
#/
#/ Dependencies:
#/ * pandoc
#/ * gpp



# Shell setup
# -----------

# Bash options for strict error checking
set -o errexit -o errtrace -o pipefail -o nounset



# Generate final documents
# ------------------------

BASEPATH="$(cd -P -- "$(dirname -- "$0")" && pwd -P)"  # Absolute canonical path

STYLE="$BASEPATH/pandoc-github.css"

for FILE in "$PWD"/*.gpp.md; do
    echo "Process file: '$FILE'"

    gpp -H -x "$FILE" \
    | pandoc --from markdown --to html5 \
        --standalone --toc \
        --self-contained --css "$STYLE" \
        --output "${FILE%%.*}.html"
done


echo "Done!"
