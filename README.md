# CppSQL

An attempt to create a minimal but highly performant interface to MySQL and SQLite databases. This
tries to match the native C connector code as closely as possible, but provide a consistent interface
where possible (it's not always possible). Speed is preferred over consistency and it's developed
to satisfy a very narrow set of use cases.

This started out because I needed something to quickly dump a MySQL database to SQLite, and my python
code using [SQLAlchemy](https://www.sqlalchemy.org) was ridiculously slow. I did some speed tests
with a few different methods and found a massive speed increase using the native C interfaces.
