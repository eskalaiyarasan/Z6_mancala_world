/// Project: Z6 Mancala World
/// File: z6_indian_classic_board.dart

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_theme/theme_template.dart';
import 'z6_indian_classic_engine.dart';
import 'z6_indian_classic.dart';
  
class Z6IndianClassicBoard extends StatefulWidget {
  final int level;
  final String player1;
  final String player2;
  final Function(int side)? onTurnChanged; // Callback used by Z6IndianClassicOpponent or others
  final Function(int side, int pitIndex)? onPitTappedCallback; // Additional custom callback hook

  const Z6IndianClassicBoard({
    super.key,
    required this.level,
    required this.player1,
    required this.player2,
    this.onTurnChanged,
    this.onPitTappedCallback,
  });

  @override
  State<Z6IndianClassicBoard> createState() => Z6IndianClassicBoardState();
}

class Z6IndianClassicBoardState extends State<Z6IndianClassicBoard> {
  late Z6IndianClassicEngine _engine;

  @override
  void initState() {
    super.initState();
    _engine = Z6IndianClassicEngine(
      level: widget.level,
      player1: widget.player1,
      player2: widget.player2,
    );
  }

  // Called by Z6IndianClassicToss via its onTossComplete callback
  Future<bool> start(String player) async {
    bool started = _engine.start(player);
    setState(() {});
    if (widget.onTurnChanged != null) {
      widget.onTurnChanged!(_engine.turn);
    }
    return started;
  }

  // Helper method used by Z6IndianClassicOpponent to check if it's the bot's turn
  bool isTurn(int sideIndex) {
    return _engine.started && !_engine.ended && _engine.turn == sideIndex;
  }

  // Helper method used by Z6IndianClassicOpponent to read pit values for a given side
  List<int> getSidePitValues(int sideIndex) {
    if (sideIndex < 0 || sideIndex >= _engine.sides.length) return [];
    return _engine.sides[sideIndex].pits.map((pit) => pit.value).toList();
  }

  // Wrapper method called by Z6IndianClassicOpponent when the AI chooses its move
  void handleAiPitTap(int sideIndex, int index) {
    _handlePitTap(sideIndex, index);
  }

  void _handlePitTap(int sideIndex, int index) async {
    if (_engine.ended) return;
    
    final theme = Provider.of<Z6ThemeProvider>(context, listen: false).currentTheme;
    int hand = 0;
    
    setState(() {
      _engine.sequence++;
      _engine.history.add("${_engine.sequence}: ($sideIndex, $index)");
    });

    if (widget.onPitTappedCallback != null) {
      widget.onPitTapped!(sideIndex, index);
    }

    while (!_engine.captured) {
      hand = _engine.sow(sideIndex, hand, index % _engine.n);
      await theme.sound.playDrop();
      sideIndex = (sideIndex + 1) % 2;
      index = 0;
    }

    if (_engine.checkValidPits(_engine.turn)) {
      _engine.roundTransit();
      if (_engine.checkValidPits(_engine.turn)) {
        setState(() {
          _engine.ended = true;
          _engine.winside = (_engine.turn + 1) % 2;
        });
        return;
      }
    }

    setState(() {
      _engine.nextTurn();
    });

    if (widget.onTurnChanged != null) {
      widget.onTurnChanged!(_engine.turn);
    }

    await theme.sound.playScoop();
  }

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;

    return Scaffold(
      appBar: AppBar(
        title: Text("Z6 Indian Classic Level ${widget.level}", style: theme.color?.text ?? Colors.black),
      ),
      drawer: Drawer(
        child: ListView(
          padding: EdgeInsets.zero,
          children: [
            DrawerHeader(
              decoration: BoxDecoration(color: theme.color?.fill ?? Colors.blue),
              child: Text("Round: ${_engine.currentRound}", style: theme.color?.text ?? Colors.black),
            ),
            ExpansionTile(
              leading: const Icon(Icons.history),
              title: const Text('View History'),
              children: [
                for (String item in _engine.history)
                  ListTile(
                    leading: const Icon(Icons.add),
                    title: Text(item),
                  ),
              ],
            ),
          ],
        ),
      ),
      body: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: !_engine.ended
            ? [
                Expanded(
                  child: Z6IndianClassicSide(
                    sideModel: _engine.sides[0],
                    isTurn: _engine.sides[0].turn && _engine.turn == 0,
                    onPitTapped: (index) => _handlePitTap(0, index % _engine.n),
                  ),
                ),
                Expanded(
                  child: Z6IndianClassicSide(
                    sideModel: _engine.sides[1],
                    isTurn: _engine.sides[1].turn && _engine.turn == 1,
                    onPitTapped: (index) => _handlePitTap(1, index % _engine.n),
                  ),
                ),
              ]
            : [
                Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    const Text("Game Ended", style: TextStyle(
                      fontSize: 20,
                      fontWeight: FontWeight.bold,
                      color: Colors.redAccent,
                    )),
                    Text("Winner: ${_engine.players[_engine.winside]}", style: const TextStyle(
                      fontSize: 20,
                      fontWeight: FontWeight.bold,
                      color: Colors.redAccent,
                    )),
                  ],
                ),
              ],
      ),
    );
  }
}