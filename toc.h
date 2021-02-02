#ifndef TOC_H
#define TOC_H

#include <QTreeWidget>
#include <poppler/qt5/poppler-qt5.h>
#include <QVector>

using Poppler::Document;
using Poppler::OutlineItem;

class Toc : public QTreeWidget{
    Q_OBJECT
public:
    Toc(Document* doc = nullptr, QWidget* parent = nullptr);
    Toc(const Toc&) = delete;
    Toc(Toc&&) = delete;
    Toc& operator=(const Toc&) = delete;
    Toc& operator=(Toc&&) = delete;
    ~Toc() = default;
    void SetDocument(Document* doc);
    void HighLightItem(int n);
private:
    void GenerateSubItem(QTreeWidgetItem* parent, const QVector<OutlineItem>& outlines);
    QVector<OutlineItem> outlineItem_;
};

#endif // TOC_H
