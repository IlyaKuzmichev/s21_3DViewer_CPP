#ifndef LINEEDITADAPTER_H
#define LINEEDITADAPTER_H

#include <QLineEdit>
#include <memory>

class LineEditAdapter : public QObject {
  Q_OBJECT
 public:
  explicit LineEditAdapter(QObject *parent, QLineEdit *line_edit)
      : QObject{parent}, line_edit_{line_edit} {}

  static std::unique_ptr<LineEditAdapter> create(QObject *parent,
                                                 QLineEdit *line_edit) {
    return std::make_unique<LineEditAdapter>(parent, line_edit);
  }

 private slots:
  void onScrollValueChanged(int value);

 private:
  QLineEdit *line_edit_;
};

#endif  // LINEEDITADAPTER_H
