#include <QAbstractListModel>
#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QBrush>
#include <QDir>
#include <QPalette>
#include "lazy_list_model.h"
#include "boyer_moore.h"

QTextStream& qStdOut() {
    static QTextStream ts( stdout );
    return ts;
}

LazyWordList::LazyWordList(QObject *parent, QString const &file_path)
    : QAbstractListModel(parent)/*, pos(1 << 8) */{
    QFile inputFile(file_path);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while (!in.atEnd()) {
           QString line = in.readLine();
           filtered_list.append(line);
           all_words.append(line);
        }
        inputFile.close();
    }
}

int LazyWordList::rowCount(const QModelIndex & /* parent */) const {
    return words_count;
}

QVariant LazyWordList::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= filtered_list.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        return filtered_list[index.row()];
    } else if (role == Qt::BackgroundRole) {
        return rand() & 1 ? qApp->palette().base() : qApp->palette().alternateBase();
    }

    return QVariant();
}

bool LazyWordList::canFetchMore(const QModelIndex & /* index */) const {
    if (words_count < filtered_list.size()) {
        return true;
    } else {
        return false;
    }
}

void LazyWordList::fetchMore(const QModelIndex & /* index */) {
    int remainder = filtered_list.size() - words_count;
    int itemsToFetch = qMin(1000, remainder);

    beginInsertRows(QModelIndex(), words_count, words_count + itemsToFetch - 1);
    words_count += itemsToFetch;
    endInsertRows();

    emit numberPopulated(itemsToFetch);
}

void LazyWordList::set_substring(const QString &s) {
    substring = s.toStdString();

    beginResetModel();
    BoyerMoore bm(substring);

    filtered_list.clear();

    for (int j = 0; j < all_words.size(); ++j) {
        QString t_qt = all_words[j];
        std::string t = t_qt.toStdString();
        if (t.size() < substring.size()) {
            continue;
        }
        // Subsequence check (OPTIONAL)
        /*
        int ps = 0, pt = 0;
        while (ps < (int) substring.size() && pt < (int) t.size()) {
            while (pt < (int) t.size() && t[pt] != s[ps]) ++pt;
            if (pt < (int) t.size()) ++ps, ++pt;
        }

        if (ps == (int) substring.size()) {
            filtered_list.append(t_qt);
            continue;
        }
        */


        // Substring check using boyer-moore algorithm (in average works in O(t.size() / substring.size()))
        if (bm.check_substring(t)) {
            filtered_list.append(t_qt);
        }
    }

    // Process stores only information about filtered records (OPTIONAL)
    /*
    QFile inputFile("/home/egor/Qt/jb-test-unreal/resources/dictionary.txt");
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while (!in.atEnd()) {
            QString t_qt = in.readLine();
            std::string t = t_qt.toStdString();
            if (t.size() < substring.size()) {
                continue;
            }
            // Subsequence check first
            int ps = 0, pt = 0;
            while (ps < (int) substring.size() && pt < (int) t.size()) {
                while (pt < (int) t.size() && t[pt] != s[ps]) ++pt;
                if (pt < (int) t.size()) ++ps, ++pt;
            }

            if (ps == (int) substring.size()) {
                filtered_list.append(t_qt);
                continue;
            }


            // Substring check using boyer-moore algorithm (in average works in O(t.size() / substring.size()))
            if (bm.check_substring(t)) {
                filtered_list.append(t_qt);
            }
        }
        inputFile.close();
    }
    */
    words_count = 0;
    endResetModel();
}
