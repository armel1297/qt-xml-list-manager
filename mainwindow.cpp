#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Verbindungen herstellen
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(testEintrag()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(eintragHinzufuegen()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(xmlSchreiben()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Slot für "Neuer Eintrag"
void MainWindow::eintragHinzufuegen()
{
    bool ok;
    QString text = QInputDialog::getText(
        this,
        "Neuer Eintrag",
        "Bitte Eintrag eingeben:",
        QLineEdit::Normal,
        "",
        &ok);

    if (ok && !text.isEmpty())
    {
        ui->listWidget->addItem(text);
    }
}

// Slot für "Und noch ein Test"
void MainWindow::testEintrag()
{
    ui->listWidget->addItem("Und noch ein Test");
}

// Slot für "Schreiben" → XML-Datei speichern
void MainWindow::xmlSchreiben()
{
    QString dateiPfad = QDir::currentPath() + "/eintraege.xml";

    QFile datei(dateiPfad);

    if (!datei.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Fehler", "Datei konnte nicht erstellt werden.");
        return;
    }

    QXmlStreamWriter xmlWriter(&datei);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // Wurzelelement
    xmlWriter.writeStartElement("Eintraege");

    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        xmlWriter.writeStartElement("Eintrag");
        xmlWriter.writeCharacters(ui->listWidget->item(i)->text());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement(); // </Eintraege>
    xmlWriter.writeEndDocument();

    datei.close();

    QMessageBox::information(this,
                             "Erfolg",
                             "XML-Datei wurde gespeichert:\n" + dateiPfad);
}
