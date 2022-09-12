TEMPLATE = subdirs

SUBDIRS += \
    app \
    arcade \
    backend \
    frontend

app.depends = arcade backend frontend
