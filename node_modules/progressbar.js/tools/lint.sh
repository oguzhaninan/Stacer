#!/bin/bash
# NOTE: Run this only from project root!

# Run all lint commands and if one fails, exit non-zero return code

EXIT_STATUS=0
npm run jscs || EXIT_STATUS=$?
npm run eslint || EXIT_STATUS=$?

exit $EXIT_STATUS
