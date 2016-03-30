#include <iostream>
#include <cassert>

#include "gameObjects.h"

void test1()
{
  AsteroidL rock;
  float * bytes = rock.toBytes();
  cout << "rock :: toBytes:" << endl;
  for (int i = 0; i < 7; i++)
  {
    cout << bytes[i] << endl;
  }

  cout << endl;

  rock.fromBytes(bytes);

  float * newBytes = rock.toBytes();

  for (int i = 0; i < 7; i++)
  {
    assert(bytes[i] == newBytes[i]);
  }
}

int main()
{
  test1();
  cout << "Passed test1" << endl;
}
