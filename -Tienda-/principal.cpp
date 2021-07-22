#include "principal.h"
#include "ui_principal.h"
#include <QDebug>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    m_productos.append(new Producto(1,"Leche",0.85));
    m_productos.append(new Producto(2,"Pan",0.15));
    m_productos.append(new Producto(3,"Queso",2.0));
    for(int i = 0; i< m_productos.size(); i++)
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    //Colocar la cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "SubTotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    //Inicializar subtotal global
    m_subtotal = 0;
}

Principal::~Principal()
{
    delete ui;
}

bool Principal::verificador(QString cedula)
{

    QString temp[10];
    int aux;
    int sumaPar = 0;
    int sumaImpar = 0;
    for(int i = 0; i < 9; i++)
    {
        temp[i] = cedula[i];
        aux = temp[i].toInt();
        if((i+1)%2==0)
            sumaPar += aux;
        else
        {
            if(aux * 2 > 9)
                sumaImpar = sumaImpar + ((aux * 2) - 9);

            else 
                sumaImpar += aux * 2;
        }
    }

 int ultimoCedula = cedula.toInt()%10;
  int verificador = (sumaPar + sumaImpar)%10;
    if(verificador !=0)
        verificador = 10 - verificador;
    return ultimoCedula == verificador? true : false;
}

void Principal::limpiar()
{
    ui->inCedula->clear();
    ui->inNombre->clear();
    ui->inTelefono->clear();
    ui->inEmail->clear();
    ui->inDireccion->clear();
    informacion.clear();
    ui->outAdvertencia->clear();
    ui->outDetalle->setRowCount(0);
    m_subtotal = 0;
    ui->outSubtotal->setText("$ " + QString::number(m_subtotal, 'f', 2));
    ui->outIva->setText("$ " + QString::number(0.0, 'f', 2));
    ui->outTotal->setText("$ " + QString::number(0.0, 'f', 2));
}


void Principal::on_cmdAgregar_released()
{
    //Obtener datos de la GUI
    int cantidad = ui->inCantidad->value();

    //Validar que no se agreguen productos con 0 cantidad
    if(cantidad == 0){
        return;
    }
    int index = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(index);

    //Calcular el subtotal del producto
    float subtotal = cantidad * p->precio();

    //Agregar datos a la tabla
    int fila = ui->outDetalle->rowCount();
    ui->outDetalle->insertRow(fila);
    ui->outDetalle->setItem(fila,0,new QTableWidgetItem(QString::number(cantidad)));
    ui->outDetalle->setItem(fila,1,new QTableWidgetItem(p->nombre()));
    ui->outDetalle->setItem(fila,2,new QTableWidgetItem("$ " + QString::number(subtotal, 'f',2)));

    //Agregar los datos de compra a la informacion
    informacion =informacion + "----------------------------"
                               "\nCant: " + QString::number(cantidad)
            + "\nProducto: " + p->nombre()
            + "\nSubTotal: $" + QString::number(subtotal,'f',2) + "\n";

    //Limpiar datos
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();

    //Invocar a calcular
    calcular(subtotal);
}


void Principal::on_inProducto_currentIndexChanged(int index)
{
    //Obtener el precio del producto
    float precio = m_productos.at(index)->precio();
    ui->outPrecio->setText("$ " + QString::number(precio,'f',2));
}

void Principal::calcular(float stProducto)
{
    m_subtotal += stProducto;
    m_iva = m_subtotal * IVA/100;
    m_total = m_subtotal + m_iva;
    ui->outSubtotal->setText("$ " + QString::number(m_subtotal, 'f', 2));
    ui->outIva->setText("$ " + QString::number(m_iva, 'f', 2));
    ui->outTotal->setText("$ " + QString::number(m_total, 'f', 2));
}

void Principal::on_cmdCompra_released()
{


    QString cedula = ui->inCedula->text();
    QString nombre = ui->inNombre->text();
    QString telf = ui->inTelefono->text();
    QString correo = ui->inEmail->text();
    QString dir = ui->inDireccion->toPlainText();
    if(informacion.isEmpty())
        ui->outAdvertencia->setText("<p style=color:#FF0000;><b>Aviso:</b> No se han agregado productos</p>");

    else if(cedula.isEmpty())
        ui->outAdvertencia->setText("<p style=color:#FF0000;><b>Error:</b> La cedula debe ser ingresada</p>");

    else if(cedula == "9999999999")
    {

        Cliente *p = new Cliente();
        Finalizar *resumen = new Finalizar();
        resumen->setCliente(p->informacion());
        resumen->setResumen(informacion);
        resumen->setSubtotal("$ " + QString::number(m_subtotal, 'f', 2));
        resumen->setIva("$ " + QString::number(m_iva, 'f', 2));
        resumen->setTotal("$ " + QString::number(m_total, 'f', 2));
        resumen->show();
        limpiar();
    }
    else if(nombre.isEmpty())
        ui->outAdvertencia->setText("<p style=color:#FF0000;><b>Error:</b> El nombre debe ser ingresado</p>");
    else if(verificador(cedula))
    {

        Cliente *p = new Cliente(cedula, nombre, telf, correo, dir);
        Finalizar *finalizar = new Finalizar();
        finalizar ->setCliente(p->informacion());
        finalizar ->setResumen(informacion);
        finalizar ->setSubtotal("$ " + QString::number(m_subtotal, 'f', 2));
        finalizar ->setIva("$ " + QString::number(m_iva, 'f', 2));
        finalizar ->setTotal("$ " + QString::number(m_total, 'f', 2));
        finalizar ->show();
        limpiar();
    }
    else if(!verificador(cedula))
        ui->outAdvertencia->setText("<p style=color:#FF0000;><b>Error:</b> La cedula que ha ingresado es Incorrecta</p>");
}
