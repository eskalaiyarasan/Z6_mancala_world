/// Project: Z6 Mancala World
/// File: z6_indian_classic_engine.dart

import 'z6_indian_classic_level.dart';
import 'base_object.dart';

class Z6IndianClassicEngine {
  final int level;
  final String player1;
  final String player2;
  
  late int n;
  late int m;
  late int pasu;
  
  int turn = 0;
  bool started = false;
  bool ended = false;
  int winside = 0;
  bool captured = false;
  bool capture2x = false;
  int capture2xIndex = -1;
  int capture2xSide = 0;
  int currentRound = 0;
  int sequence = 0;

  late List<Z6SideModel> sides;
  final Map<int, String> players = {};
  final List<String> history = [];

  Z6IndianClassicEngine({
    required this.level,
    required this.player1,
    required this.player2,
  }) {
    n = Z6IndianClassicLevel.get('n', level);
    m = Z6IndianClassicLevel.get('m', level);
    pasu = Z6IndianClassicLevel.get('pasu', level);
    
    players[0] = player1;
    players[1] = player2;

    sides = List.generate(
      2,
      (sideIndex) => Z6SideModel(
        side: sideIndex,
        n: n,
        m: m,
        turn: (sideIndex == 0),
      ),
    );
  }

  bool start(String player) {
    if (started) return false;
    if (players[0] == player || players[1] == player) {
      started = true;
      turn = 0;
      return true;
    }
    return false;
  }

  void roundTransit() {
    for (var side in sides) {
      int hand = side.store.retrieve();
      for (var pit in side.pits) {
        if (pit.value > 0) {
          hand += pit.scoop();
        }
      }
      for (var pit in side.pits) {
        hand = pit.reset(hand);
      }
      if (hand > 0) side.store.store(hand);
    }
    currentRound++;
  }

  bool checkValidPits(int sideIndex) {
    for (var pit in sides[sideIndex].pits) {
      if (pit.value > 0) return false;
    }
    return true;
  }

  void nextTurn() {
    turn = (turn + 1) % 2;
    captured = false;
    for (var side in sides) {
      collectPasu(side.side);
    }
  }

  void collectPasu(int sideIndex) {
    if (pasu > 0) {
      int targetSide = (sideIndex + 1) % 2;
      for (var pit in sides[targetSide].pits) {
        if (pit.value == pasu) {
          int seeds = pit.scoop();
          sides[targetSide].store.store(seeds);
        }
      }
    }
  }

  int front(int sideIndex, int j) {
    if (j >= 0 && j < n - 1) {
      for (int i = j + 1; i < n; i++) {
        if (sides[sideIndex].pits[i].value >= 0) return i;
      }
    }
    return -1;
  }

  void capture(int sideIndex, int index) {
    captured = true;
    int next = front(sideIndex, index);
    if (capture2x) {
      int doubleSeeds = sides[capture2xSide].pits[capture2xIndex].scoop();
      sides[turn].store.store(doubleSeeds);
      capture2x = false;
    }
    if (next < 0) {
      sideIndex = (sideIndex + 1) % 2;
      next = front(sideIndex, 0);
    }
    if (next >= 0) {
      int seeds = sides[sideIndex].pits[next].scoop();
      sides[turn].store.store(seeds);
    }
  }

  int sow(int sideIndex, int hand, int index) {
    if (index < 0 || index >= n) return hand;
    for (int j = index; j < n; j++) {
      if (sides[sideIndex].pits[j].value == -1) continue;
      if (hand == 0) {
        hand = sides[sideIndex].pits[j].scoop();
        if (hand == 0) {
          capture(sideIndex, j);
          break;
        }
      } else {
        hand--;
        sides[sideIndex].pits[j].inc();
        if (pasu > 0 && sides[sideIndex].pits[j].value == pasu) {
          capture2x = true;
          capture2xIndex = j;
          capture2xSide = sideIndex;
        }
      }
    }
    return hand;
  }
}