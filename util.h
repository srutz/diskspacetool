#ifndef UTIL_H
#define UTIL_H

#include <QWidget>
#include <QMainWindow>
#include <vector>

using namespace std;


struct Util {

    static QMainWindow* getMainWindow(QWidget *widget);

    template<typename V, typename T>
    static int indexOf(vector<V>& v, T val) {
        for (int i = 0; i < v.size(); i++) {
            if (v[i].get() == val) {
                return i;
            }
        }
        return -1;
    }

    static void openLocalFile(const QString &file);

};

#endif // UTIL_H
