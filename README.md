# CppSQL

An attempt to create a minimal but highly performant interface to MySQL and SQLite databases. This
tries to match the native C connector code as closely as possible, but provide a consistent interface
where possible (it's not always possible). Speed is preferred over consistency and it's developed
to satisfy a very narrow set of use cases. You'll probably need to know a little bit about the MySQL
and SQLite C connectors, so you should view this just as a few tools to help with that rather than a
complete abstraction.

This started out because I needed something to quickly dump a MySQL database to SQLite, and my python
code using [SQLAlchemy](https://www.sqlalchemy.org) was ridiculously slow. I did some speed tests
with a few different methods and found a massive speed increase using the native C interfaces. This
is just the C connector test [from those speed tests](https://github.com/mark-grimes/dbSaveTest)
spun out into a separate project.

# Licence

I don't really care what you do with the parts I've written, I guess I'll put it out under MIT. If you
particularly want a different licence I could change it given a decent reason. You can use either the
MySQL or the MariaDB connector, but I think the MySQL one is GPL which would make the whole project
subject to the GPL. So if that bothers you use the MariaDB one. I'm not a patent lawyer though so
don't take my word for it, it's up to you to figure that stuff out for yourself.
