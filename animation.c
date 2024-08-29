#include "header.h"
#include "medicine.h"
#include "read.h"
#define liquid_key 0x1236
#define pills_key 0x1238

#define screenWidth 800
#define screenHeight 1000
#define rectWidth 0.085
#define rectHeight 0.85

Numbers_liquid_lines *shared_liquid;
Numbers_Pill_lines *shared_pilles;

int Liquid_generator = 0;
int Pilles_generatr = 0;
int Liquid_generator2 = 0;
float redX = -1; // initial x cordinate position for red square
float greenX = -1.2;
float spacing = 0.05;
int initial_num_of_pilles = 0;
int num_of_pilles = 150; // from shared mem
int inital_num_liquid = 0;

int num_of_liquid; //--------------------from shared mem
int rej;
float stopGreen = 100;
float stopRed = 100;
int counter_lin1_package;
int counter_lin2_package;
int package_pilles = 0;
int package_liquid = 0;

int num_package_liquid;
int initial_num_package_liquid = 0;
int num_package_liquid2;
int remainingTime;
int frame_rate = 1200;
float gap = 0.05;
int inital_num_liquid2 = 0;
int num_of_liquid2;
int initial_num_of_pilles2 = 0;
int num_of_pilles2 = 150;
int initial_num_package_liquid2 = 0;

int rejected_liq_line1;
int rejected_liq_line2;
int rejected_pills_line1;
int rejected_pills_line2;

int dif;
int rej2;
int dif2;
int employee_liq_line1;
int employee_liq_line2;
int employee_pills_line1;
int employee_pills_line2;

int pills_rejected1_count;
int pills_employee1_count;
int pills_rejected2_count;
int pills_employee2_count;

int difp;
int difp2;
int rejp;
int rejp2;
//-------------------

void increase_liquid_square(int value);
void increase_pilles_square(int value);
void initTimer();
void timer(int value);
void incrementLiquidPackage(int value);
void incrementPillesPackage(int value);
void update(int value);
void init();
void display();
void increase_liquid_square2(int value);
void increase_pilles_square2(int value);
void incrementLiquidPackage2(int value);
//---------------------------------------------------------------------------

void initTimer() {
  remainingTime = 180 * 1000; // 3 minutes in milliseconds
}

void timer(int value) {
  if (remainingTime > 0) {
    remainingTime -= 1000; // Decrement the timer by one second each call
  }

  glutTimerFunc(frame_rate, timer,
                0);    // Re-register timer for continuous updates
  glutPostRedisplay(); // Request a redraw to update the timer display
}

// Liquid Line 1
//*************************************
//  Draw Liquid square
void increase_liquid_square(int value) {
  num_of_liquid = shared_liquid[1].num_generated;
  // printf("iam heeeeeeeeeeeere %d", num_of_liquid);
  if (inital_num_liquid <
      num_of_liquid) { // Assuming you want to draw 5 squares
                       // printf("iam heeeeeeeeeeeere %d", num_of_liquid);
    inital_num_liquid++;
    Liquid_generator++;
    glutPostRedisplay(); // Redraw the window
    glutTimerFunc(4000, increase_liquid_square,
                  0); // Call again after 1000 milliseconds (1 second)
  }
}

void incrementLiquidPackage(int value) {
  num_of_liquid = shared_liquid[1].num_generated;
  package_liquid = shared_liquid[1].num_packaged;
  rej = shared_liquid[1].num_rejected;
  dif = num_of_liquid - rej;
  if (initial_num_package_liquid < dif) {
    initial_num_package_liquid++;
    counter_lin1_package++;
    glutPostRedisplay();
  }

  glutTimerFunc(5000, incrementLiquidPackage, 0);
}

// Liquid Line 2
//*************************************

// Draw Liquid square
void increase_liquid_square2(int value) {
  num_of_liquid2 = shared_liquid[2].num_generated;
  if (inital_num_liquid2 <
      num_of_liquid2) { // Assuming you want to draw 5 squares

    inital_num_liquid2++;
    Liquid_generator2++;
    glutPostRedisplay(); // Redraw the window
    glutTimerFunc(7000, increase_liquid_square2,
                  0); // Call again after 1000 milliseconds (1 second)
  }
}

void incrementLiquidPackage2(int value) {
  num_of_liquid2 = shared_liquid[2].num_generated;
  num_package_liquid2 = shared_liquid[2].num_packaged;
  rej2 = shared_liquid[2].num_rejected;
  dif2 = num_of_liquid2 - rej2;
  if (initial_num_package_liquid2 < dif2) {
    initial_num_package_liquid2++;
    counter_lin2_package++;
    glutPostRedisplay();
  }

  glutTimerFunc(8000, incrementLiquidPackage2, 0);
}

// Variables for Pills Line 1
int initial_num_pills = 0;
int Pills_generator1 = 0;
int counter_pills_line1_package = 0;
int initial_num_package_pills1 = 0;
int num_of_pills1 = 0;
int num_package_pills1 = 0;

// Variables for Pills Line 2
int initial_num_pills2 = 0;
int Pills_generator2 = 0;
int counter_pills_line2_package = 0;
int initial_num_package_pills2 = 0;
int num_of_pills2 = 0;
int num_package_pills2 = 0;

// Pills Line 1
//*************************************
// Draw Pills square
void increase_pills_square(int value) {
  num_of_pills1 = shared_pilles[1].num_generated;
  if (initial_num_pills < num_of_pills1) {
    initial_num_pills++;
    Pills_generator1++;
    glutPostRedisplay(); // Redraw the window
    glutTimerFunc(4000, increase_pills_square,
                  0); // Call again after 4000 milliseconds (4 seconds)
  }
}

void incrementPillsPackage(int value) {
  num_package_pills1 = shared_pilles[1].num_packaged;
  num_of_pills1 = shared_pilles[1].num_generated;
  rejp = shared_pilles[1].num_rejected;
  difp = num_of_pills1 - rejp;
  if (initial_num_package_pills1 < difp) {
    initial_num_package_pills1++;
    counter_pills_line1_package++;
    glutPostRedisplay();
  }
  glutTimerFunc(5000, incrementPillsPackage,
                0); // Call again after 2500 milliseconds (2.5 seconds)
}

// Pills Line 2
//*************************************
// Draw Pills square
void increase_pills_square2(int value) {
  num_of_pills2 = shared_pilles[2].num_generated;
  if (initial_num_pills2 < num_of_pills2) {
    initial_num_pills2++;
    Pills_generator2++;
    glutPostRedisplay(); // Redraw the window
    glutTimerFunc(7000, increase_pills_square2,
                  0); // Call again after 7000 milliseconds (7 seconds)
  }
}

void incrementPillsPackage2(int value) {
  num_package_pills2 = shared_pilles[2].num_packaged;
  num_of_pills2 = shared_pilles[2].num_generated;
  rejp2 = shared_pilles[2].num_rejected;
  difp2 = num_of_pills2 - rejp2;
  if (initial_num_package_pills2 < difp2) {
    initial_num_package_pills2++;
    counter_pills_line2_package++;
    glutPostRedisplay();
  }
  glutTimerFunc(8000, incrementPillsPackage2,
                0); // Call again after 2500 milliseconds (2.5 seconds)
}

//-----------------------------------------------------------
void update(int value) {
  if (greenX <= stopGreen - rectWidth) {
    greenX += 0.001;
  }
  if (redX <= stopRed - rectWidth) {
    redX += 0.001;
  }
  glFlush();
  usleep(1000); // Add a small delay to the animation
  glutPostRedisplay();

  glutTimerFunc(25, update, 0); // Call update after 10 milliseconds
}

void init() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

// -------------------- Display --------------------------------

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  char buffer[32];
  glColor3f(1, 0, 1);
  glRasterPos2f(-0.3, 0.85); // Move the position more to the left

  glutBitmapString(
      GLUT_BITMAP_TIMES_ROMAN_24,
      (const unsigned char *)" Pharmaceutical medicine inspection  ");

  glColor3f(0.0, 0.0, 0.0); // set color to gray

  // Add Liquid Line: text
  glRasterPos2f(-0.85, 0.75); // update the position
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,
                   (const unsigned char *)"Liquid Line: ");

  // Add "Pills Line" text
  glRasterPos2f(-0.85, 0.18); // update the position
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,
                   (const unsigned char *)"Pills Line: ");

  //----------------------------------------------------------------------------
  // First rectangle liquid with decreased height, light purple
  glColor3f(1.0, 0.5, 1.0); // Set color to light purple
  glBegin(GL_POLYGON);
  glVertex2f(-1.0, 0.7); // top-left vertex
  glVertex2f(-1.0, 0.5); // bottom-left vertex
  glVertex2f(1.0, 0.5);  // bottom-right vertex
  glVertex2f(1.0, 0.7);  // top-right vertex
  glEnd();

  // Draw green squares centered vertically within the rectangle
  // Set color to blue (comment said green, but code uses blue)
  float centerY = 0.6;                 // Center Y position of the squares
  float squareBottom = centerY - 0.05; // Bottom Y of the squares
  float squareTop = centerY + 0.05;    // Top Y of the squares
  int startRedIndexx = inital_num_liquid - shared_liquid[1].num_rejected;
  for (int i = 0; i < inital_num_liquid; i++) {
    if (i >= startRedIndexx) {
      glColor3f(1.0, 0.0, 0.0); // Set color to red for squares from the last up
                                // to the rejected count
    } else {
      glColor3f(0.0, 0.0, 1.0); // Set color to blue for other squares
    }
    float squareX = greenX - i * (rectWidth + spacing);
    glBegin(GL_POLYGON);
    glVertex2f(squareX, squareBottom);
    glVertex2f(squareX, squareTop);
    glVertex2f(squareX + rectWidth, squareTop);
    glVertex2f(squareX + rectWidth, squareBottom);
    glEnd();
  }

  //--------------------------------------------------------------------------------------
  // Second rectangle liquid, adjusted to match the first rectangle's height and
  // moved up
  glColor3f(1.0, 0.5, 1.0); // Set color to light purple
  glBegin(GL_POLYGON);
  glVertex2f(-1.0, 0.45); // top-left vertex
  glVertex2f(-1.0, 0.25); // bottom-left vertex
  glVertex2f(1.0, 0.25);  // bottom-right vertex
  glVertex2f(1.0, 0.45);  // top-right vertex
  glEnd();

  // Draw squares centered vertically within the second rectangle (blue)
  glColor3f(0.0, 0.0, 1.0);              // Set color to blue
  float centerY2 = 0.35;                 // Center Y position of the squares
  float squareBottom2 = centerY2 - 0.05; // Bottom Y of the squares
  float squareTop2 = centerY2 + 0.05;    // Top Y of the squares
  int startRedIndex =
      inital_num_liquid2 -
      shared_liquid[2].num_rejected; // Calculate the start index for red
                                     // squares from the end
  for (int i = 0; i < inital_num_liquid2; i++) {

    if (i >= startRedIndex) {
      glColor3f(1.0, 0.0, 0.0); // Set color to red for squares from the last up
                                // to the rejected count
    } else {
      glColor3f(0.0, 0.0, 1.0); // Set color to blue for other squares
    }
    float squareX = greenX - i * (rectWidth + spacing);
    glBegin(GL_POLYGON);
    glVertex2f(squareX, squareBottom2);
    glVertex2f(squareX, squareTop2);
    glVertex2f(squareX + rectWidth, squareTop2);
    glVertex2f(squareX + rectWidth, squareBottom2);
    glEnd();
  }

  //*****************************************
  // pills rectangles
  // First pills

  // Adjusted first rectangle moved further up
  glColor3f(1.0, 0.5, 1.0); // Set color to light purple
  glBegin(GL_POLYGON);
  glVertex2f(-1.0, -0.10 + 0.25); // top-left vertex, shifted up by 0.10
  glVertex2f(-1.0, -0.30 + 0.25); // bottom-left vertex, shifted up by 0.10
  glVertex2f(1.0, -0.30 + 0.25);  // bottom-right vertex, shifted up by 0.10
  glVertex2f(1.0, -0.10 + 0.25);  // top-right vertex, shifted up by 0.10
  glEnd();

  // Add five red squares inside the significantly lowered second rectangle

  float rectBottom = .2;    // Bottom y-coordinate of the rectangle
  float rectTop = -0.10;    // Top y-coordinate of the rectangle
  float squareHeight = 0.1; // Height of each square
  float squareYBottom =
      rectBottom + (rectTop - rectBottom) / 2 -
      squareHeight / 2; // Center the square vertically within the rectangle
  float squareYTop = squareYBottom + squareHeight;
  int startRedpilles = initial_num_pills - shared_pilles[1].num_rejected;

  for (int i = 0; i < initial_num_pills; i++) {
    if (i >= startRedpilles) {
      glColor3f(1.0, 0.0, 0.0); // Set color to red for squares from the last up
                                // to the rejected count
    } else {
      glColor3f(0.0, 0.0, 1.0); // Set color to blue for other squares
    }
    glBegin(GL_POLYGON);
    // Calculate the x-coordinate by subtracting from the starting x-position
    float squareX = redX - i * (rectWidth + spacing);
    glVertex2f(squareX, squareYBottom);
    glVertex2f(squareX, squareYTop);
    glVertex2f(squareX + rectWidth, squareYTop);
    glVertex2f(squareX + rectWidth, squareYBottom);

    glEnd();
  }
  //-------------------

  // Second pills
  float newTop = -0.07;            // Top vertex y-coordinate
  float newBottom = newTop - 0.20; // Bottom vertex y-coordinate calculated to
                                   // ensure the rectangle height is 0.20

  // Second rectangle with the same color
  glColor3f(1.0, 0.5, 1.0); // Set color to light purple
  glBegin(GL_POLYGON);
  glVertex2f(-1.0, newTop); // top-left vertex
  glVertex2f(-1.0,
             newBottom); // bottom-left vertex, adjusted for correct height
  glVertex2f(1.0,
             newBottom);   // bottom-right vertex, adjusted for correct height
  glVertex2f(1.0, newTop); // top-right vertex
  glEnd();

  float squareYBottom2 =
      newBottom + (newTop - newBottom) / 2 - squareHeight / 2;
  float squareYTop2 = squareYBottom2 + squareHeight;
  int startRedpilles2 = initial_num_pills2 - shared_pilles[2].num_rejected;

  for (int i = 0; i < initial_num_pills2; i++) {
    if (i >= startRedpilles2) {
      glColor3f(1.0, 0.0, 0.0); // Set color to red for squares from the last up
                                // to the rejected count
    } else {
      glColor3f(0.0, 0.0, 1.0); // Set color to blue for other squares
    }
    float squareX = redX - i * (rectWidth + spacing);
    glBegin(GL_POLYGON);
    glVertex2f(squareX, squareYBottom2);
    glVertex2f(squareX, squareYTop2);
    glVertex2f(squareX + rectWidth, squareYTop2);
    glVertex2f(squareX + rectWidth, squareYBottom2);

    glEnd();
  }

  // Set text color to black for all text
  glColor3f(0.0, 0.0, 0.0); // Set color to black

  glRasterPos2f(-0.99, -0.4); // Position for the label "Label Line 1"
  char label_line1[50];
  sprintf(label_line1, "Liquid Line 1: "); // Formulating the label text
  for (char *c = label_line1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  glRasterPos2f(-0.7, -0.4);
  char liquid_label[50];
  sprintf(liquid_label, "Generator: %d", Liquid_generator);

  for (char *c = liquid_label; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the count of liquid packages
  glRasterPos2f(-0.45, -0.4);
  char liquid_info[50];
  sprintf(liquid_info, "Packages : %d", counter_lin1_package);
  for (char *c = liquid_info; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Rejected label
  glRasterPos2f(-0.2, -0.4); // Adjust the position as needed
  char rejected_label[50];
  sprintf(rejected_label, "Rejected: %d", shared_liquid[1].num_rejected);
  for (char *c = rejected_label; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Employee label
  glRasterPos2f(0.1, -0.4); // Adjust the position as needed
  char employee_label[50];
  sprintf(employee_label, "Employee: %d", shared_liquid[1].num_employees);
  for (char *c = employee_label; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  glRasterPos2f(-0.99, -0.5); // Position for the label "Label Line 1"
  char label_line2[50];
  sprintf(label_line2, "Liquid Line 2: "); // Formulating the label text
  for (char *c = label_line2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Calculate approximate width to place the second line beside the first
  glRasterPos2f(
      -0.7, -0.5); // Adjust the position as needed // Pixel width to viewport
                   // width, adjust divisor based on your viewport width

  char liquid_label2[50];
  sprintf(liquid_label2, "Generator: %d",
          Liquid_generator2); // Replace with actual variable if different

  for (char *c = liquid_label2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the count of liquid packages
  glRasterPos2f(-0.45, -0.5);
  char liquid_info2[50];
  sprintf(liquid_info2, "Packages : %d", counter_lin2_package);
  for (char *c = liquid_info2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
  // Display the Rejected label for Liquid Line 2
  glRasterPos2f(-0.2, -0.5); // Adjust the position as needed
  char rejected_label2[50];
  sprintf(rejected_label2, "Rejected: %d", shared_liquid[2].num_rejected);
  for (char *c = rejected_label2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Employee label for Liquid Line 2
  glRasterPos2f(0.1, -0.5); // Adjust the position as needed
  char employee_label2[50];
  sprintf(employee_label2, "Employee: %d", shared_liquid[2].num_employees);
  for (char *c = employee_label2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  //-----------------------------------------------------------------------------------------
  // Set text color to black for all text
  glColor3f(0.0, 0.0, 0.0); // Set color to black

  // Display for Pills Line 1
  glRasterPos2f(
      -0.99, -0.6); // Adjusted position slightly lower than the liquid labels
  char pills_label1[50];
  sprintf(pills_label1, "Pills Line 1: "); // Formulating the label text
  for (char *c = pills_label1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Assume Pills_line1_generator is a variable storing the generator count for
  // Pills Line 1
  glRasterPos2f(-0.7, -0.6);
  char pills_generator1[50];
  sprintf(pills_generator1, "Generator: %d", Pills_generator1);
  for (char *c = pills_generator1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Assume counter_pills_line1_package is a variable storing the package count
  // for Pills Line 1
  glRasterPos2f(-0.45, -0.6);
  char pills_info1[50];
  sprintf(pills_info1, "Packages : %d", counter_pills_line1_package);
  for (char *c = pills_info1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Rejected label for Pills Line 1
  glRasterPos2f(-0.2, -0.6); // Adjust the position as needed
  char pills_rejected1[50];
  sprintf(pills_rejected1, "Rejected: %d", shared_pilles[1].num_rejected);
  for (char *c = pills_rejected1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Employee label for Pills Line 1
  glRasterPos2f(0.1, -0.6); // Adjust the position as needed
  char pills_employee1[50];
  sprintf(pills_employee1, "Employee: %d", shared_pilles[1].num_employees);
  for (char *c = pills_employee1; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display for Pills Line 2
  glRasterPos2f(-0.99, -0.7); // Further down
  char pills_label2[50];
  sprintf(pills_label2, "Pills Line 2: "); // Formulating the label text
  for (char *c = pills_label2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Assume Pills_line2_generator is a variable storing the generator count for
  // Pills Line 2
  glRasterPos2f(-0.7, -0.7);
  char pills_generator2[50];
  sprintf(pills_generator2, "Generator: %d", Pills_generator2);
  for (char *c = pills_generator2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Assume counter_pills_line2_package is a variable storing the package count
  // for Pills Line 2
  glRasterPos2f(-0.45, -0.7);
  char pills_info2[50];
  sprintf(pills_info2, "Packages : %d", counter_pills_line2_package);
  for (char *c = pills_info2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
  // Display the Rejected label for Pills Line 2
  glRasterPos2f(-0.2, -0.7); // Adjust the position as needed
  char pills_rejected2[50];
  sprintf(pills_rejected2, "Rejected: %d", shared_pilles[2].num_rejected);
  for (char *c = pills_rejected2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  // Display the Employee label for Pills Line 2
  glRasterPos2f(0.1, -0.7); // Adjust the position as needed
  char pills_employee2[50];
  sprintf(pills_employee2, "Employee: %d", shared_pilles[1].num_employees);
  for (char *c = pills_employee2; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }

  int minutes = remainingTime / 60000;
  int seconds = (remainingTime / 1000) % 60;
  char timerText[50];
  sprintf(timerText, "Time Left: %02d:%02d", minutes, seconds); // Format text

  glRasterPos2f(-0.9, 0.85); // Set the position for the text
  for (char *c = timerText; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

// -------------------- Main ------------------------------

int main(int argc, char **argv) {

  // SHARED MEM FOR liquid
  int shmid1 =
      shmget(liquid_key, sizeof(Numbers_liquid_lines), IPC_CREAT | 0666);
  if (shmid1 == -1) {
    perror("shmget for liquid lines failed");
    exit(EXIT_FAILURE);
  }

  // Attach to the shared liquid
  shared_liquid = (Numbers_liquid_lines *)shmat(shmid1, NULL, 0);
  if (shared_liquid == (Numbers_liquid_lines *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  // sharedmemory for pilles
  int shmid = shmget(pills_key, sizeof(Numbers_Pill_lines), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  shared_pilles = (Numbers_Pill_lines *)shmat(shmid, NULL, 0);
  if (shared_pilles == (Numbers_Pill_lines *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  // for loop for liquid line 1 and 2 values
  for (int i = 0; i < 700; i++) {

    // For liquid
    num_of_liquid = shared_liquid[1].num_generated;

    num_of_liquid2 = shared_liquid[2].num_generated;
    package_liquid = shared_liquid[1].num_packaged;
    num_package_liquid2 = shared_liquid[2].num_packaged;

    rejected_liq_line1 = shared_liquid[1].num_rejected;
    rejected_liq_line2 = shared_liquid[2].num_rejected;
    employee_liq_line1 = shared_liquid[1].num_employees;
    employee_liq_line2 = shared_liquid[2].num_employees;

    num_of_pills1 = shared_pilles[1].num_generated;
    num_package_pills1 = shared_pilles[1].num_packaged;
    num_of_pills2 = shared_pilles[2].num_generated;
    num_package_pills2 = shared_pilles[2].num_packaged;
  }

  //-----------------------------------------------------------------
  glutInit(&argc, argv);
  glutTimerFunc(0, update, 0); // Start the animation timer

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(screenHeight, screenWidth);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Project#3_1200837_1200335_1200183_1200255");
  init();
  glutDisplayFunc(display);

  glutTimerFunc(25000, increase_liquid_square, 0); // Start the delay timer
  glutTimerFunc(25000, increase_liquid_square2, 0);
  glutTimerFunc(55000, incrementLiquidPackage, 0);
  glutTimerFunc(55000, incrementLiquidPackage2, 0);

  glutTimerFunc(25000, increase_pills_square, 0); // Start the delay timer
  glutTimerFunc(25000, increase_pills_square2, 0);
  glutTimerFunc(55000, incrementPillsPackage, 0);
  glutTimerFunc(55000, incrementPillsPackage2, 0);

  initTimer(); // Initialize the timer

  glutTimerFunc(1000, timer, 0);
  glutMainLoop();

  return 0;
}
