#!/bin/sh
# Navigate to data directory
cd "$(dirname "$0")/../data" || exit 1

rm -f user_settings.ini
rm -f sbitx.db

if [ -f "create_db.sql" ]; then
    sqlite3 sbitx.db < create_db.sql
else
    echo "Error: create_db.sql not found."
fi
