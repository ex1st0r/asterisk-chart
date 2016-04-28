#include <stdio.h>
#include <sys/ioctl.h>
#include <math.h>

class Point{
  public: double x, y;
};

class Chart
{
  private: int rows, columns;
  private: int arr_n, arr_m;
  private: double cell_width, cell_height;
  private: double x_start, x_end, y_start, y_end;
  private: double len_x, len_y;
  private: bool **cells;
  private: Point *arr_points;

  //Constructor
  public: Chart(Point *arr, double x_s, double x_e, double y_s, double y_e, int n, int m){
    struct winsize size;
    ioctl(0, TIOCGWINSZ , &size);
    printf ("rows %d\n", size.ws_row);
    printf ("columns %d\n", size.ws_col);
    rows =  size.ws_row;
    columns = size.ws_col;
    arr_points = arr;

    x_start = x_s;
    x_end = x_e;
    y_start = y_s;
    y_end = y_e;
    arr_n = n;
    arr_m = m;

   //Steps of approximation
   len_x = (x_end - x_start) / (double) columns;
   len_y = (y_end - y_start) / (double) rows;

    cells = new bool*[rows];
    for(int i = 0; i < rows; i++){
      cells[i] = new bool[columns];
    }

    fillCells();
    printCells();
  }

  private: void printCells(){
    for(int i = rows - 1; i >= 0; i--){
      for(int j = 0; j < columns; j++){
        if(cells[i][j] == true){
          printf("*");
        } else {
          printf("#");
        }
      }
    }
  }

  private: void fillCells(){
    int i, j;
    for(i = 0; i < rows; i++){
      for(j = 0; j < columns; j++){
        if (inArr(x_start+j*len_x, x_start+(j+1)*len_x, y_start+i*len_y,y_start+(i+1)*len_y)) {
              cells[i][j] = true;
            }
      }
    }
  }

  private: bool inArr(double x_s,double x_e,double y_s,double y_e){
    Point * arr = arr_points;
    for(int i = 0; i < arr_n; i++){
        if (x_s <= arr[i].x 
            && (x_e) > arr[i].x
            && (y_s) <= arr[i].y
            && (y_e) > arr[i].y) {
              return true;
            }
    }
    return false;
  }

};

//Function
Point func(double x){
  Point point;
  point.x = x;
  point.y = x*x*(x-2)*(x-2);
  return point;
};

//Function approximate
Point* approx_func(double x_start, double x_end, double y_start, double y_end, int n, int m){
  int i,j;
  Point * arr = new Point[n];
  double len_x = (x_end - x_start) / (double) n, len_y = (y_end - y_start) / (double) m;
  for (i = 0; i < n; i++){
    arr[i] = func(x_start + len_x * i);
  }
  return arr;
};

int main(void)
{
  double x_start = -2,x_end = 3, y_start = 0, y_end = 10;
  int n = 500,m = 500;
  Point *arr;
  arr = approx_func(x_start, x_end, y_start, y_end, n, m);
  Chart chart(arr, x_start, x_end, y_start, y_end, n, m);

  return 0;
}
