#include "xml_editor.h"
#include "ui_xml_editor.h"

XML_Editor::XML_Editor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XML_Editor)
{
    ui->setupUi(this);
}

XML_Editor::~XML_Editor()
{
    delete ui;
}

