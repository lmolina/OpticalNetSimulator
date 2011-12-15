# include <QApplication>
# include <grid_window.H>

int main(int argc, char * argv[])
{
  QApplication app(argc, argv);
  Grid_Window win;
  win.show();
  return app.exec();
}
