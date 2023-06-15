#include "weatherdialog.h"

WeatherDialog::WeatherDialog()
{
    this->resize(600, 300);
    this->setMinimumSize(600, 300);
    this->setWindowTitle("ChangeCity");

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->setAlignment(Qt::AlignHCenter);
    cityList<<"tianjin"<<"beijing"<<"shanghai"<<"chongqing";
    comboBox=new QComboBox();
    for(int i=0; i<4; i++) {
        comboBox->addItem(cityList[i]);
    }
    comboBox->setEditable(true);
    submit=new QPushButton("切换城市");

    vbl->addWidget(submit,1);
    vbl->addWidget(comboBox, 1);
    this->setLayout(vbl);
    QObject::connect(submit,SIGNAL(clicked()),this,SLOT(changeCity()));
}

void WeatherDialog::changeCity(){
    chosenCity=comboBox->currentText();
    this->close();
    emit closeDialog(chosenCity);
}
