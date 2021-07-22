#include "finalizar.h"
#include "ui_finalizar.h"

void Finalizar::setSubtotal(QString subtotal)
{
    ui->outSubtotal->setText(subtotal);
}

void Finalizar::setIva(QString iva)
{
    ui->outIva->setText(iva);
}

void Finalizar::setTotal(QString total)
{
    ui->outTotal->setText(total);
    m_total = total;
}

Finalizar::Finalizar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finalizar)
{
    ui->setupUi(this);
}

Finalizar::~Finalizar()
{
    delete ui;
}

void Finalizar::setCliente(QString cliente)
{
    ui->outCliente->setPlainText(cliente);
}

void Finalizar::setResumen(QString informacion)
{
    ui->outResumen->setPlainText(informacion);
}

void Finalizar::on_cmdGuardar_released()
{
    QDir directorio = QDir::home();
    QString pathArchivo = directorio.absolutePath() + "Venta.txt";
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo", pathArchivo, "Archivo de texto(*.txt)");
    QFile f(fileName);
    QTextStream out(&f);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QMessageBox::warning(this, "Resumen", "No se puede abrir el archivo");
        return;
    }
    out << "Cliente\n " << ui->outCliente->toPlainText() << "\nResumen de la compra\n" << ui->outResumen->toPlainText() << "\nTotal: " << m_total << "\n------------";
    f.close();
    this->hide();
}
