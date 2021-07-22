#ifndef FINALIZAR_H
#define FINALIZAR_H
#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
class Finalizar;
}

class Finalizar : public QWidget
{
    Q_OBJECT

public:
    explicit Finalizar(QWidget *parent = nullptr);
    ~Finalizar();
    void setCliente(QString cliente);
    void setResumen(QString informacion);
    void setSubtotal(QString subtotal);
    void setIva(QString iva);
    void setTotal(QString total);

private slots:
    void on_cmdGuardar_released();

private:
    Ui::Finalizar *ui;
    QString m_total;
};

#endif // FINALIZAR_H
