/// Project: Z6 Mancala World
/// File: z6_indian_classic_engine.dart

import 'level.dart';
import 'base_object.dart';

class Z6IndianClassicEngine {
  final int level;
  final String player1;
  final String player2;

  late int n;
  late int m;
  late int pasu;

  int turn = -1;
  bool started = false;
  bool ended = false;
  int winside = 0;
  bool captured = false;
  bool capture2x = false;
  int capture2xIndex = -1;
  int capture2xSide = 0;
  int currentRound = 0;
  int sequence = 0;
  static const int Z = 2;
  late List<Z6SideModel> sides;
  final Map<int, String> players = {};
  final List<String> history = [];
  late Z6MoveSummary _moveSummary;
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
      Z,
      (sideIndex) => Z6SideModel(side: sideIndex, n: n, m: m, turn: false),
    );
    _initMoveSummary();
  }

  void _initMoveSummary() {
    _moveSummary = Z6MoveSummary();
    for (var side in sides) {
      for (var x in side.pits) {
        _moveSummary.add(
          x.side,
          x.index.toString(),
          x.value,
          Z6ActionType.init,
        );
      }
      _moveSummary.add(
        side.side,
        Z6Action.store,
        side.store.value,
        Z6ActionType.store,
      );
    }
  }

  Z6MoveSummary getMoveSummary() {
    return _moveSummary;
  }

  bool start(String player) {
    print('\@Z6IndianClassicEngine -> \@start : $player , $started');
    bool ret = false;
    if (started) return ret;
    if (players[0] == player) {
      turn = 0;
      ret = true;
    } else if (players[1] == player) {
      turn = 1;
      ret = true;
    }
    started = ret;
    currentRound = 1;
    for (var side in sides) {
      if (side.side == turn) {
        side.turn = true;
      } else {
        side.turn = false;
      }
    }
    print('\@Z6IndianClassicEngine -> start\@ : $player , $started , $turn');
    return false;
  }

  void roundTransit() {
    print('\@Z6IndianClassicEngine -> \@roundTransit ');
    for (var side in sides) {
      int hand = side.store.retrieve();
      for (var pit in side.pits) {
        if (pit.value > 0) {
          int seeds = pit.scoop();
          hand += seeds;
          _moveSummary.add(
            pit.side,
            pit.index.toString(),
            0,
            Z6ActionType.scoop,
          );
          _moveSummary.add(
            pit.side,
            pit.index.toString(),
            seeds,
            Z6ActionType.capture0x,
          );
        }
      }
      for (var pit in side.pits) {
        hand = pit.reset(hand);
        _moveSummary.add(
          pit.side,
          pit.index.toString(),
          pit.value,
          pit.active ? Z6ActionType.roundup : Z6ActionType.ghost,
        );
      }
      if (hand > 0) {
        side.store.store(hand);
        _moveSummary.add(
          side.side,
          Z6Action.store,
          side.store.value,
          Z6ActionType.store,
        );
      }
    }
    currentRound++;
  }

  bool checkValidPits(int sideIndex) {
    print('\@Z6IndianClassicEngine -> \@checkValidPits: $sideIndex ');
    for (var pit in sides[sideIndex].pits) {
      if (pit.active && (pit.value > 0)) return false;
    }
    return true;
  }

  void nextTurn() {
    print('\@Z6IndianClassicEngine -> \@nextTurn: $turn ');
    turn = (turn + 1) % Z;
    captured = false;
    for (var side in sides) {
      collectPasu(side.side);
      if (side.side == turn) {
        side.turn = true;
      } else {
        side.turn = false;
      }
    }
    if (checkValidPits(turn)) {
      roundTransit();
      if (checkValidPits(turn)) {
        ended = true;
        _moveSummary.add(turn, Z6Action.all, 0, Z6ActionType.finish);
      }
    }
  }

  void collectPasu(int sideIndex) {
    print('\@Z6IndianClassicEngine -> \@collectPasu: $sideIndex ');
    if (pasu > 0) {
      int targetSide = (sideIndex + 1) % Z;
      for (var pit in sides[targetSide].pits) {
        if (pit.value == pasu) {
          int seeds = pit.scoop();
          sides[targetSide].store.store(seeds);
          _moveSummary.add(
            targetSide,
            pit.index.toString(),
            0,
            Z6ActionType.scoop,
          );
          _moveSummary.add(
            targetSide,
            pit.index.toString(),
            pasu,
            Z6ActionType.pasu,
          );
          _moveSummary.add(
            targetSide,
            Z6Action.store,
            sides[targetSide].store.value,
            Z6ActionType.store,
          );
        }
      }
    }
  }

  int front(int sideIndex, int j, bool k) {
    print('\@Z6IndianClassicEngine -> \@front: $sideIndex ,$j , $k');
    if (j >= 0 && j < n - 1) {
      for (int i = k ? j : j + 1; i < n; i++) {
        if (sides[sideIndex].pits[i].value >= 0) return i;
      }
    }
    return -1;
  }

  void capture(int sideIndex, int index) {
    print('\@Z6IndianClassicEngine -> \@capture: $sideIndex , $index');
    captured = true;
    int next = front(sideIndex, index, false);
    if (capture2x) {
      int doubleSeeds = sides[capture2xSide].pits[capture2xIndex].scoop();
      sides[turn].store.store(doubleSeeds);
      capture2x = false;
      _moveSummary.add(
        capture2xSide,
        capture2xIndex.toString(),
        0,
        Z6ActionType.scoop,
      );
      _moveSummary.add(turn, Z6Action.all, pasu, Z6ActionType.capture2x);
      _moveSummary.add(
        capture2xSide,
        Z6Action.store,
        sides[turn].store.value,
        Z6ActionType.store,
      );
    }
    if (next < 0) {
      sideIndex = (sideIndex + 1) % Z;
      next = front(sideIndex, 0, true);
    }
    if (next >= 0) {
      int seeds = sides[sideIndex].pits[next].scoop();
      sides[turn].store.store(seeds);
      _moveSummary.add(sideIndex, next.toString(), 0, Z6ActionType.scoop);
      _moveSummary.add(turn, Z6Action.all, seeds, Z6ActionType.capture);
      _moveSummary.add(
        turn,
        Z6Action.store,
        sides[turn].store.value,
        Z6ActionType.store,
      );
    }
  }

  Z6MoveSummary move(int sideIndex, int index) {
    print('\@Z6IndianClassicEngine -> \@move: $sideIndex,$index ');
    if (index < 0 || index >= n) return Z6MoveSummary();
    bool pasuFlag = true;
    bool captureFlag = false;
    int j = index;
    int hand = 0;
    _moveSummary.reset();
    history.add("( $sideIndex , $index )");
    while (captureFlag == false) {
      if (j >= n) {
        sideIndex = (sideIndex + 1) % Z;
        j = 0;
        pasuFlag = true;
      }
      if (sides[sideIndex].pits[j].value == -1) continue;
      if (hand == 0) {
        hand = sides[sideIndex].pits[j].scoop();
        if (hand == 0) {
          captureFlag = true;
          capture(sideIndex, j);
          break;
        } else {
          capture2x = false;
          _moveSummary.add(sideIndex, j.toString(), 0, Z6ActionType.scoop);
        }
      } else {
        hand--;
        sides[sideIndex].pits[j].inc();
        var tempv = sides[sideIndex].pits[j].value;
        _moveSummary.add(sideIndex, j.toString(), tempv, Z6ActionType.drop);
        if (pasu > 0 && tempv == pasu) {
          capture2x = true;
          capture2xIndex = j;
          capture2xSide = sideIndex;
        } else {
          capture2x = false;
        }
      }
      if (pasuFlag) {
        pasuFlag = false;
        collectPasu(sideIndex);
      }
    }
    return _moveSummary;
  }
}
