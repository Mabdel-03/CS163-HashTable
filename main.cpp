/* Code Written By Mahmoud Abdelmoneum
 * Code Written on 4/29/2021
 * This code builds on the StudentList Program using Hashtables 
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;
#define SIZE 100

struct Student{
  char fName[20];
  char lName[20];
  int id;
  float GPA;
  Student* next;
};

//function prototypes
vector<Student>* randStudent(int numOfStudents, vector<Student> *studentVtr, int &startingID);
Student** addStudent(vector<Student> *studentVtr, Student *array[], int &size);
void deleteStudent(Student *array[], int deleteid, int size);
void printStudent(Student *array[], vector<Student> *studentVtr, int &size);
bool isCommandLegal(char* command);
void quit();
Student** movePtr(Student *newArray[], Student *addPtr, int size);
bool checkNeedHash(Student *array[], int size);  
void eraseVector(vector<Student> *studentVtr);
Student** rehash(Student *array[], int &size);
int myhash(Student *searchStudent, int &size);

//goes through array and sees if hash is needed
bool checkNeedHash(Student *array[], int size){
  //rehash needed if over half of table is filled
  int amount = 0; //amount of students
  int collisions = 0;
  Student *traverse = NULL;
  for(int i = 0; i < size; i++){
    collisions = 0;
    
    if(array[i] != NULL){ //there is a student in that slot
      traverse = array[i];
      amount++;
      while(traverse->next != NULL){ //it has a next
	traverse = traverse->next;
	amount++;
	collisions++; 
      }
    }
    if (collisions > 3) { //too many collisions
      cout << "\nToo many collisions, needs rehash!\n";
      return true;
    }
  }
  //other case: if amount of students is over half the size of array...
  if (amount > (size/2)){
    return true;
  } else {
    return false;
  }
}
		   
//myhash returns the array slot of the student
int myhash(Student *searchStudent, int &size){
  Student **array = new Student*[size];
  Student *studentPtr = NULL;
  int arrayslot = 0;
  int nameLen = 0; //length of both first and last name of a student
  nameLen = strlen(searchStudent->fName) + strlen(searchStudent->lName);
  //hash function: id % size
  arrayslot = (searchStudent->id) % size;
  return arrayslot;
}

//function to move a current and prev through linked list in the array
Student** movePtr(Student *newArray[], Student *addPtr, int size){
  Student *traverse = NULL;
  Student *prevPtr = NULL;
  int slot = 0;
  slot = myhash(addPtr, size);
  traverse = newArray[slot];

  // Adding to head of linked list in slot
  if (traverse == NULL) {
    newArray[slot] = addPtr;
    addPtr->next = NULL;
  } else { //not head
    while (traverse != NULL) { //traverse through list, bring prev along
      prevPtr = traverse;
      traverse = traverse->next;
    }
    //link up chain
    prevPtr->next = addPtr;
    addPtr->next = NULL;
  }
    return newArray;
}

//rehashes all students into new array double size of original if collision count > 3
// called by manual add in main and by addStudent()
Student** rehash(Student *array[], int &size){
  size *= 2; //double array size
  int arraySlot = 0;
  Student **newArray = new Student *[size]; //create new array
  Student *next = NULL;
  for(int j = 0; j < size; j++){ //fill new array with nulls to start
    newArray[j] = NULL;
  }
  
  Student *traverse = NULL;
  for (int i = 0; i < (size/2); i++){ //traverse original array
    traverse = array[i];
    if(traverse != NULL){
      next = traverse->next; //move to next
      newArray = movePtr(newArray, traverse, size);
      traverse = next;
      
      while(traverse != NULL){ //while there's something to traverse
	if (traverse != NULL) {
	  next = traverse->next; //keep moving
	  newArray = movePtr(newArray, traverse, size); //save to new array
	  traverse = next;
	}
      }
    }
  }
  return newArray;
}

int main(){
  vector<Student> *studentVtr = new vector<Student>;
  int collisionCount = 0;
  int size = SIZE;
  bool needhash = false;
  Student** array = new Student*[size];
  int startingID = 0;
  char command[10];
  bool neededHash = false;
  //populate array with nulls
  for(int i = 0; i < SIZE; i++){
    array[i] = NULL;
  }
  //welcome messages
  cout << "********************** " << endl;
  cout << "Welcome to Student List - hash table edition!" << endl; 
  cout << "To add, type 'ADD'. To delete, type 'DELETE'." << endl;
  cout << "To print existing students, type PRINT. To quit, type QUIT." << endl;
  
  while (strcmp(command, "QUIT") != 0){
    cout << "********************** " << endl;
    cout << "Enter a command: " << endl;
    cout << "(ADD/DELETE/PRINT/QUIT)" << endl;
    cin.get(command, 10);
    cin.get();

    //format to all uppercase
    for(int i = 0; i < strlen(command); i++){
      command[i] = toupper(command[i]);
    }

    //check to see that command is legal                                                 
    bool legal = isCommandLegal(command);
    if (legal == true){
      if (strcmp(command, "ADD") == 0){
	//give options
	cout << "Would you like to manually add students or randomly generate? (man/rand)" << endl;
	char manOrRand[5];
	cin.get(manOrRand, 5);
	cin.get();

	//randomly generate from files
	if(strcmp(manOrRand, "rand") == 0){
	  cout << "How many students would you like to randomly generate? " << endl;
	  int numOfStudents = 0;
	  cin >> numOfStudents;
	  cin.get();  
	  studentVtr = randStudent(numOfStudents, studentVtr, startingID);
	  array = addStudent(studentVtr, array, size);
	  eraseVector(studentVtr);
	}
	else if(strcmp(manOrRand, "man") == 0){
	  //collect data
	  cout << "Adding manually. " << endl;
	  Student *newStudent = new Student;
	  char f[20];
	  char l[20];
	  float gpa = 0.00;
	  int id = 0;
	  cout << "Enter the first name of the student: " << endl;
	  cin.get(f, 20);
	  cin.get();
	  strcpy(newStudent->fName, f);
	  cout << "Enter the last name of the student: " << endl;
	  cin.get(l, 20);
	  cin.get();
	  strcpy(newStudent->lName, l);
	  cout << "Enter the GPA of the student: " << endl;
	  cin >> gpa;
	  cin.get();
	  newStudent->GPA = gpa;
	  cout << "Enter the ID of the student: " << endl;
	  cin >> id;
	  cin.get();
	  newStudent->id = id;

	  //add this student into the array
	  int index = 0;
	  index = myhash(newStudent, size);

	  //if there is a collision  
	  if(array[index] != NULL){
	    collisionCount++;
	    cout <<"collision found with index: " << index << endl;
	    cout << "collision count: " << collisionCount << endl;
	    Student *traverse = array[index];
	    while(traverse->next != NULL){ //find end of chain
	      traverse = traverse->next;
	    }
	    //link up chain
	    traverse->next = newStudent;
	    newStudent->next = NULL;
	    startingID++;
	  } else { //no collision
	    array[index] = newStudent;
	    startingID++;
	  }	  
	}
	//	printStudent(array, studentVtr, size); 
	neededHash = checkNeedHash(array, size);
	if(neededHash == true){ //if needs to hash, then rehash
	  array = rehash(array, size);
	  printStudent(array, studentVtr, size);
	}
      }
      //call print
      else if (strcmp(command, "PRINT") == 0){
        printStudent(array, studentVtr, size);
      }

      //call delete
      else if (strcmp(command, "DELETE") == 0){
	cout << "What is the student ID of the student you would like to delete?" << endl;
	int deleteid = 0;
	cin >> deleteid;
	cin.get();
	deleteStudent(array, deleteid, size);
      }
    }
  }
  quit();
  return 0;
}



//randomly generate students (that for some reason have celebrity names)
//uses .txt files full of random names
vector<Student>* randStudent(int numOfStudents, vector<Student> *studentVtr, int &startingID){
  ifstream inFile; //first names
  ifstream inFile2; //last names
  int totalLines = 0;
  vector<char*> fNameVtr;
  vector<char*> lNameVtr;
  char line[20];
  int id = startingID;
  srand(time(0));
  bool uniqueNum = true;
  char* templine = NULL;
  Student* traverse = NULL;
  cout << fixed;
  cout << setprecision(2);
  
  //access files
  inFile.open("firstNames.txt");
  inFile2.open("lastnames.txt");

  if(!inFile){  //error out if cannot open
    cout << "Unable to open first name file" << endl;
    exit(1);
  }
  if(!inFile2){ //error out if cannot open
    cout << "Unable to open last name file" << endl;
    exit(1);
  }

  //count num of lines in first name file to choose random line later
  while(inFile >> line){
    totalLines++;
    templine = new char[strlen(line)+1]; //+1 is for terminating char
    strcpy(templine, line);
    fNameVtr.push_back(templine); //add to first name vtr
  }

  //create random student first name, id, and GPA
  for (int i = 0; i < numOfStudents; i++){
    //create random GPA
    float gpa = static_cast <float> (rand())/static_cast <float> (RAND_MAX/4);
    Student* newstudent = new Student;
    int randNum = rand()%totalLines; //random number is created to choose name
    strcpy(newstudent->fName, fNameVtr[randNum]);
    newstudent->id = id; //set id
    newstudent->GPA = gpa; //set gpa
    studentVtr->push_back(*newstudent); //add to vtr
    startingID++;
    id++; //increment id
  }

  //now onto last names
  totalLines = 0; //reset total lines to 0
  while(inFile2 >> line){
    totalLines++;
    templine = new char[strlen(line)+1]; //save line being read
    strcpy(templine, line);
    lNameVtr.push_back(templine); //add to last name vtr
  }
  //add last name to corresponding first student in list
  for (int k = 0; k < numOfStudents; k++){
    int randNum = rand()%totalLines;
    traverse = &(studentVtr->at(k)); //set traverse ptr to current student
    //set that student's last name to random index created
    strcpy(traverse->lName, lNameVtr[randNum]);
  }
  
  /*for debug: print out all the students that were created randomly
  for(vector<Student>::iterator index = studentVtr->begin(); index != studentVtr->end(); ++index){
    cout << "\n" << index->fName << " " << index->lName << ", " << index->id << ", " <<
      index->GPA << endl;
  }
  */
  return studentVtr;
}

//quits program (obviously)
void quit(){
  cout << "Now quitting. Goodbye!" << endl;
}

//walks through vector and deletes every item in it
void eraseVector(vector<Student> *studentVtr){
  studentVtr->clear();
}
//take in student vector, populate array with students
Student** addStudent(vector<Student> *studentVtr, Student *array[], int &size){
  int index = 0;
  bool needhash = false;
  int collisionCount = 0;
  Student *studentPtr = NULL;
  Student *prev = NULL;

  //traverse vector
  for(vector<Student>::iterator i = studentVtr->begin(); i != studentVtr->end(); ++i){
    Student *newStudent = new Student;

    //copy over all data
    newStudent->id = i->id;
    strcpy(newStudent->fName, i->fName);
    strcpy(newStudent->lName, i->lName);
    newStudent->GPA = i->GPA;
    newStudent->next = NULL;
    index = myhash(newStudent, size); //save hash index
        
    //if there is a collision  
    if(array[index] != NULL){
      collisionCount++;
      Student *traverse = array[index];
      while(traverse != NULL){ //find end of chain
	prev = traverse;
	traverse = traverse->next;
      }
       //link up chain
      prev->next = newStudent;
      newStudent->next = NULL;
    } else {
      array[index] = newStudent;
    }
  }
  return array;
}



//pass in the id you would like to delete, delete that student and rehash all
void deleteStudent(Student *array[], int deleteid, int size){
  Student *studentPtr = NULL;
  Student *temp = NULL;
  int index = 0;
  int nullCount = 0; //used to see if array is empty
  bool breakflag = false;
  for (int j = 0; j < size; j++){ //check if array is empty
    if(array[j] == NULL){
      nullCount++;
    }
  }
  if(nullCount == size){ //if it's full of nulls, then cout and return
    cout << "There are no students to delete!" << endl;
    return;
  }

  //walk thru array 
  for(int i = 0; i < size; i++){
    studentPtr = array[i];

    //if the student has no chain and matches id
    if(studentPtr != NULL && studentPtr->id == deleteid){
      Student *ptr = array[i]->next; //make backup temp ptr
      delete array[i];
      array[i] = ptr;
      break;
    }

    //if student is in a chain
    while(studentPtr != NULL && studentPtr->next != NULL){
      if(studentPtr->next->id == deleteid){ //matching id?
	temp = studentPtr->next;
	studentPtr->next = studentPtr->next->next;
	delete temp;
	breakflag = true;
	break; //get out of while loop
      }
      studentPtr = studentPtr->next; //increment
    }
    if(breakflag == true){ //get out of for loop
      break;
    }
  }
}
void printStudent(Student *array[], vector<Student> *studentVtr, int &size){
  Student *traverse = NULL;
  cout << "\nStudents....\n\n";
  for(int i = 0; i < size; i++){ //traverse array
    traverse = array[i];
    if(traverse != NULL){ //not null meaning that there is something there
      cout << i << " " << traverse->fName << " " << traverse->lName << ", " << traverse->id << ", " << traverse->GPA << endl;
      while(traverse->next != NULL){ //traverse through linked list
	traverse = traverse->next;
	cout << " - " << traverse->fName << " " << traverse->lName << ", " << traverse->id << ", " << traverse->GPA << endl;
      }
    } 
  }
  cout << "\tSize of hash table is " << size << endl;
}

//compares if command given is one of the 4 possible commands
bool isCommandLegal(char*command){
  if (strcmp(command, "ADD") == 0){
    return true;
  }
  if (strcmp(command, "DELETE") == 0){
    return true;
  }
  if (strcmp(command, "QUIT") == 0){
    return true;
  }
  if (strcmp(command, "PRINT") == 0){
    return true;
  }
  //else it is not one of the 4 commands                                               
  return false;
}

