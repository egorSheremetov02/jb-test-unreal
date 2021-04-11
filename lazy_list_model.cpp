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
           filteredList.append(line);
           allWords.append(line);
        }
        inputFile.close();
    }
}

int LazyWordList::rowCount(const QModelIndex & /* parent */) const {
    return wordsCount;
}

QVariant LazyWordList::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= filteredList.size() || index.row() < 0)
        return QVariant();
    if (role == Qt::DisplayRole) {
        return filteredList[index.row()];
    } else if (role == Qt::BackgroundRole) {
        return rand() & 1 ? qApp->palette().base() : qApp->palette().alternateBase();
    }

    return QVariant();
}

bool LazyWordList::canFetchMore(const QModelIndex & /* index */) const {
    if (wordsCount < filteredList.size()) {
        return true;
    } else {
        return false;
    }
}

void LazyWordList::fetchMore(const QModelIndex & /* index */) {
    int remainder = filteredList.size() - wordsCount;
    int wordsToFetch = qMin(1000, remainder);

    beginInsertRows(QModelIndex(), wordsCount, wordsCount + wordsToFetch - 1);
    wordsCount += wordsToFetch;
    endInsertRows();

    emit numberPopulated(wordsToFetch);
}

void LazyWordList::setSubstring(const QString &s) {
    substring = s.toStdString();

    beginResetModel();
    BoyerMoore bm(substring);

    filteredList.clear();

    for (int j = 0; j < allWords.size(); ++j) {
        QString tq = allWords[j];
        std::string t = tq.toStdString();
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
            filteredList.append(tq);
            continue;
        }
        */


        // Substring check using boyer-moore algorithm (in average works in O(t.size() / substring.size()))
        if (bm.checkSubstring(t)) {
            filteredList.append(tq);
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
                filteredList.append(t_qt);
                continue;
            }


            // Substring check using boyer-moore algorithm (in average works in O(t.size() / substring.size()))
            if (bm.checkSubstring(t)) {
                filteredList.append(t_qt);
            }
        }
        inputFile.close();
    }
    */
    wordsCount = 0;
    endResetModel();
}
