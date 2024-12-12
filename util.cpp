#include "util.h"


QMainWindow* Util::getMainWindow(QWidget *widget) {
    return qobject_cast<QMainWindow *>(widget->window());
}



