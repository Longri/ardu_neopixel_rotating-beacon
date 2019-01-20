/*
   Copyright 2019 Longri

   This program is free software: you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
   PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with
   this program. If not, see <http://www.gnu.org/licenses/>.

   Created by Longri on 17.01.2019.
*/
#ifndef ROTATING_BEACON_SEQUENCEFUNCTIONARRAY_H
#define ROTATING_BEACON_SEQUENCEFUNCTIONARRAY_H

#include <Function.h>

class SequenceFunctionArray {
  private:
    int size = 0;
    int length = 5;
    vl::Func<bool ()> *items = new vl::Func<bool ()>[length];

    /** Creates a new backing array with the specified size containing the current items. */
    vl::Func<bool ()> *resize (int newSize) {
      vl::Func<bool ()>* items = this->items;
      vl::Func<bool ()>* newItems = new vl::Func<bool ()>[newSize];
      arraycopy(items, 0, newItems, 0, minInt(size, newSize));
      this->items = newItems;
      return newItems;
    }

    static void arraycopy(vl::Func<bool ()>* src, long srcPos, vl::Func<bool ()>* dest, long destPos, long length) {
      //      memcpy(dest[destPos], src[srcPos], static_cast<size_t>(length * sizeof(vl::Func<bool ()>)));
    }

    int minInt(int a, int b) {
      return (a <= b) ? a : b;
    }

    int maxInt(int a, int b) {
      return (a >= b) ? a : b;
    }

  public:

    void add (vl::Func<bool ()> value) {
      vl::Func<bool ()>* items = this->items;
      if (size == length) items = resize(maxInt(8, (int)(size * 1.75f)));
      items[size++] = value;
    }

    vl::Func<bool ()> get (int index) {
      return items[index];
    }

    int getSize() {
      return size;
    }

    // Operators

    vl::Func<bool ()>operator[](int idx) {
      return items[idx];
    }

};


#endif //ROTATING_BEACON_SEQUENCEFUNCTIONARRAY_H
