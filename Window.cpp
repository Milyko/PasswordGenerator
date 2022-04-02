#include "Window.h"

Window::Window(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *configLayout = new QHBoxLayout(this);
    mainLayout->addLayout(configLayout);

    this->initPasswordLengthLayout(configLayout);
    this->initOptionsLayout(configLayout);
    this->initGenerationLayout(mainLayout);
}

void Window::copy(){
#if QT_CONFIG(clipboard)
    this->mPasswordField->selectAll();
    this->mPasswordField->copy();
#endif
}

void Window::generate(){
    int length = this->mPasswordLengthSlider->value();

    QString *password = this->generatePassword(length);

    this->mPasswordField->setText(*password);

    if(this->mOptionCopy->checkState() == Qt::Checked){
        this->copy();
    }
}

QString* Window::generatePassword(int length){
    QString *password = new QString();

    if(this->mOptionLowercase->checkState() != Qt::Checked &&
            this->mOptionUppercase->checkState() != Qt::Checked &&
            this->mOptionNumber->checkState() != Qt::Checked &&
            this->mOptionSymbols->checkState() != Qt::Checked){
        std::cout << "Failed to generate password: No option has been selected" << std::endl;
        return password;
    }

    while(password->size() < length){
        int random = std::rand() % 94 + 33;
        if(this->mOptionLowercase->checkState() != Qt::Checked &&
                random >= 97 && random <= 122){
            continue;
        }else if(this->mOptionUppercase->checkState() != Qt::Checked &&
                 random >= 65 && random <= 90){
            continue;
        }else if(this->mOptionNumber->checkState() != Qt::Checked &&
                 random >= 48 && random <= 57){
            continue;
        }else if(this->mOptionSymbols->checkState() != Qt::Checked &&
                 (random < 48) ||
                 (random > 57 && random < 65) ||
                 (random > 90 && random < 97) ||
                 (random > 122)){
            continue;
        }else if(this->mOptionExcludeAmbiguous->checkState() == Qt::Checked &&
                 this->isAmbiguousChar(random)){
            continue;
        }
        password->append((char)random);
    }

    return password;
}

void Window::initGenerationLayout(QVBoxLayout *parentLayout){
    QHBoxLayout *generationLayout = new QHBoxLayout();
    parentLayout->addLayout(generationLayout);

    QPushButton *generationButton = new QPushButton(tr("Générer"));
    generationLayout->addWidget(generationButton);

    QObject::connect(generationButton, SIGNAL(clicked()), this, SLOT(generate()));

    this->mPasswordField = new QLineEdit();
    generationLayout->addWidget(this->mPasswordField);

    QPushButton *selectButton = new QPushButton(tr("Copier"));
    generationLayout->addWidget(selectButton);
    QObject::connect(selectButton, SIGNAL(clicked()), this, SLOT(copy()));
}

void Window::initOptionsLayout(QHBoxLayout *mainLayout){
    QGroupBox *groupBox = new QGroupBox("Options");
    mainLayout->addWidget(groupBox);

    QVBoxLayout *optionsLayout = new QVBoxLayout();
    groupBox->setLayout(optionsLayout);

    this->mOptionLowercase = new QCheckBox(tr("Lettres minuscules"));
    optionsLayout->addWidget(this->mOptionLowercase);
    this->mOptionLowercase->setCheckState(Qt::Checked);

    this->mOptionUppercase = new QCheckBox(tr("Lettres majuscules"));
    optionsLayout->addWidget(this->mOptionUppercase);
    this->mOptionUppercase->setCheckState(Qt::Checked);

    this->mOptionNumber = new QCheckBox(tr("Chiffres"));
    optionsLayout->addWidget(this->mOptionNumber);
    this->mOptionNumber->setCheckState(Qt::Checked);

    this->mOptionSymbols = new QCheckBox(tr("Symboles et caractères spéciaux"));
    optionsLayout->addWidget(this->mOptionSymbols);
    this->mOptionSymbols->setCheckState(Qt::Checked);

    this->mOptionExcludeAmbiguous = new QCheckBox(tr("Exclure les caractères ambigüs"));
    optionsLayout->addWidget(this->mOptionExcludeAmbiguous);
    this->mOptionExcludeAmbiguous->setCheckState(Qt::Unchecked);

    this->mOptionCopy = new QCheckBox(tr("Copier automatiquement dans le presse-papier"));
    optionsLayout->addWidget(this->mOptionCopy);
    this->mOptionCopy->setCheckState(Qt::Checked);
}

void Window::initPasswordLengthLayout(QHBoxLayout *mainLayout){
    QGroupBox *groupBox = new QGroupBox("Longueur du mot de passe");
    mainLayout->addWidget(groupBox);

    QVBoxLayout *lengthLayout = new QVBoxLayout();
    groupBox->setLayout(lengthLayout);

    this->mPasswordLengthSlider = new QSlider();
    lengthLayout->addWidget(this->mPasswordLengthSlider);
    this->mPasswordLengthSlider->setRange(0, 512);
    this->mPasswordLengthSlider->setOrientation(Qt::Horizontal);

    QHBoxLayout *passwordLengthLayout = new QHBoxLayout();
    lengthLayout->addLayout(passwordLengthLayout);

    QLabel *passwordLengthLabel = new QLabel();
    passwordLengthLabel->setText(tr("Longueur : "));
    passwordLengthLayout->addWidget(passwordLengthLabel);

    this->mPasswordLengthField = new QLineEdit();
    passwordLengthLayout->addWidget(this->mPasswordLengthField);
    this->mPasswordLengthField->setValidator(new QIntValidator(0, 512, this));

    QObject::connect(this->mPasswordLengthSlider, SIGNAL(valueChanged(int)), this, SLOT(setPasswordLength(int)));
    this->mPasswordLengthSlider->setValue(32);

    QObject::connect(this->mPasswordLengthField, SIGNAL(textChanged(QString)),
                     this, SLOT(setPasswordLengthSlider(QString)));
}

bool Window::isAmbiguousChar(int charCode){
    switch (charCode){
    case (int)'{':
    case (int)'}':
    case (int)'[':
    case (int)']':
    case (int)'(':
    case (int)')':
    case (int)'/':
    case (int)'\\':
    case (int)'\'':
    case (int)'"':
    case (int)'`':
    case (int)'~':
    case (int)',':
    case (int)';':
    case (int)':':
    case (int)'.':
    case (int)'<':
    case (int)'>':
        return true;
    default:
        return false;
    }
}

void Window::setPasswordLength(int length){
    this->mPasswordLengthField->setText(std::to_string(length).c_str());
}

void Window::setPasswordLengthSlider(const QString value){
    this->mPasswordLengthSlider->setValue(value.toInt());
}
