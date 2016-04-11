#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnMenu_clicked();

    void on_txtPag_textChanged();

    void on_btnOmitir_clicked();

    void on_btnAgregar_clicked();

    void on_cbPalabras_currentIndexChanged(int index);

    void on_lwSug_doubleClicked(const QModelIndex &index);

    void on_sbTam_valueChanged(int arg1);

    void on_cbFont_currentIndexChanged(int index);

protected:
    void timerEvent(QTimerEvent *event);

public slots:
    void abrir();

    void guardar();

    void guardarComo();

    void nuevo();

    void reload();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
