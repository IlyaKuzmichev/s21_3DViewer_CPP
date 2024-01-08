#ifndef SCROLLBARADAPTER_H
#define SCROLLBARADAPTER_H

#include <QObject>
#include <QtWidgets/QScrollBar>
#include <memory>

class ScrollBarAdapter : public QObject {
  Q_OBJECT
 public:
  explicit ScrollBarAdapter(QObject *parent, QScrollBar *scroll_bar)
      : QObject{parent}, scroll_bar_(scroll_bar) {}

  static std::unique_ptr<ScrollBarAdapter> create(QObject *parent,
                                                  QScrollBar *scroll_bar) {
    return std::make_unique<ScrollBarAdapter>(parent, scroll_bar);
  }

 private slots:
  void onLineTranslateEditingFinished();
  void onLineRotateEditingFinished();

 private:
  QScrollBar *scroll_bar_;
};

#endif  // SCROLLBARADAPTER_H
