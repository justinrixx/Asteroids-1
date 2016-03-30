#include <iostream>
#include <cassert>

#include "gameObjects.h"

void test1()
{
  Ship rock;
  float * bytes = rock.toBytes();
  cout << "rock :: toBytes:" << endl;
  for (int i = 0; i < 6; i++)
  {
    cout << bytes[i] << endl;
  }

  cout << endl;

  rock.fromBytes(bytes);

  float * newBytes = rock.toBytes();

  for (int i = 0; i < 6; i++)
  {
    if (bytes[i] != newBytes[i])
    {
       cout << "i: " << i << endl;
       cout << "bytes: " << bytes[i] << " newBytes: " << newBytes[i] << endl;
    }
  }
}

int main()
{
  test1();
  cout << "Passed test1" << endl;
}
