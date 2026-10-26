/// Project: Z6 Mancala World
/// File: base_object.dart

class Z6Pit {
  int index;
  int value; 
  int side;
  bool active;
  late int resetvalue;
        
  Z6Pit({
    required this.index,
    required this.value,
    required this.active,
    required this.side,
  }) {
    resetvalue = value;
  }

  void inc() {
    if (active) value += 1;
  }

  int scoop() {
    int x = -1; 
    if (active) {
      x = value;
      value = 0;
    }
    return x;
  }

  int reset(int data) {
    int ret = data; 
    if (data >= resetvalue) {
      value = resetvalue;
      active = true;
      ret = data - value;
    } else {
      value = -1;
      active = false;
    }
    return ret;
  }
}

class Z6Store {
  int side;
  int value;

  Z6Store({required this.side}) : value = 0;

  void store(int v) => value += v;

  int retrieve() {
    int x = value;
    value = 0;
    return x;
  }
}

class Z6SideModel {
  int side;
  int n;
  int m;
  Z6Store store;
  bool turn;
  late List<Z6Pit> pits;    

  Z6SideModel({
    required this.side,
    required this.n,
    required this.m,
    Z6Store? store,
    this.turn = false,
  }) : store = store ?? Z6Store(side: side) {
    // Index starts from (side * n) and increments sequentially for each pit
    pits = List.generate(
      n, 
      (i) => Z6Pit(
        index: (side * n) + i,
        value: m,
        active: true,
        side: side,
      ),
    );
  }
}