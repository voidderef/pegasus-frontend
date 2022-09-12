TEMPLATE = subdirs

SUBDIRS += \
    app \
    arcade \
    backend \
    frontend

app.depends = backend frontend
backend.depends = arcade