#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <QApplication>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <string>

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void copy();
    void generate();
    QString* generatePassword(int length);
    void setPasswordLength(int length);
    void setPasswordLengthSlider(const QString value);

signals:

private:
    void initGenerationLayout(QVBoxLayout *parentLayout);
    void initOptionsLayout(QHBoxLayout *mainLayout);
    void initPasswordLengthLayout(QHBoxLayout *mainLayout);

    bool isAmbiguousChar(int charCode);

    QSlider *mPasswordLengthSlider;
    QLineEdit *mPasswordLengthField;

    QCheckBox *mOptionLowercase;
    QCheckBox *mOptionUppercase;
    QCheckBox *mOptionNumber;
    QCheckBox *mOptionSymbols;

    QCheckBox *mOptionExcludeAmbiguous;

    QCheckBox *mOptionCopy;

    QLineEdit *mPasswordField;
};

#endif // WINDOW_H
