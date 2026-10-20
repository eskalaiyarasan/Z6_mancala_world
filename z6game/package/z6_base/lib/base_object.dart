/// 
/// Project: Z6 Mancala World
/// File: theme_template.dart
/// Author:  Kalaiyarasan
/// Date:  13/10/2026
/// Package:  Z6Base
/// 
/// Notice: All information contained herein is, and remains the property of 
/// Zerasix Gaming Studio LLP. The intellectual and technical concepts contained 
/// herein are proprietary to Zerasix Gaming Studio LLP and may be covered by 
/// Indian and foreign patents or trademarks, and are protected by trade secret 
/// or copyright law. Dissemination of this information or reproduction of this 
/// material is strictly forbidden unless prior written permission is obtained 
/// from Zerasix Gaming Studio LLP.

class Z6Pit{
    int index;
    int value; 
    int side;
    bool active;
    int resetvalue;
        
    Z6Pit({
      required this.index,
      required this.value,
      required this.active,
      required this .side}){
        resetvalue = value;
      }

    void inc(){
      if(active)
        value += 1;
    }
    int scoop(){
      int x = -1; 
      if(active){
        x= value;
        value = 0;
      }
      return x;
    }
    int reset(int data){
      if(data >= resetvalue){
        value = resetvalue;
        active = true;
      } else {
          value = -1;
          active = false;
      }
      return value;
    }
    
}

class Z6Store {
  int side;
  int value;

  Z6Store({ required this.side }){
    value = 0;
  }
  void store(int v){
      value += v;
  }

  int retrive(){
      int x = value;
      value = 0;
      return x;
  }
}


class Z6Side {
    int side;
    int n;   //n pits
    int m;   //m seeds
    Z6Store store;
    bool turn;
    late List<Z6Pit> pits;    

    Z6Side({
      required this.side,
      required this.n,
      required this.m,
      Z6Store? store,
      this.turn = false,
    }):store = store ?? Z6Store(side:side) {
        pits = List.generate(n , 
          (i) => Z6Pit( index: (side*n) + i,
                  value:m,
                  active: true),
        );
    }

    void shift(){
      turn = !turn;
    }
}
class Z6Board {
  int n; // Number of pits per side
  int m; // Number of seeds per pit
  int turn; // Holds which side is currently active (e.g., 0 or 1)
  late List<Z6Side> sides;

  Z6Board({
    required this.n,
    required this.m,
    this.turn = 0, // Default active turn is side 0
  }) {
    // Create 2 sides (side 0 and side 1)
    sides = List.generate(
      2,
      (sideIndex) => Z6Side(
        side: sideIndex,
        n: n,
        m: m,
        turn: (sideIndex == turn), // Set turn flag true for the active side
      ),
    );
  }

  // Method to switch turns between side 0 and side 1
  void switchTurn() {
    turn = (turn == 0) ? 1 : 0;
    for (int i = 0; i < sides.length; i++) {
      sides[i].turn = (sides[i].side == turn);
    }
  }
}
