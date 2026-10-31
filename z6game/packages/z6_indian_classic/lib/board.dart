/// Project: Z6 Mancala World
/// File: z6_indian_classic_board.dart

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_indian_classic/base_object.dart';
import 'package:z6_theme/theme_template.dart';

import 'engine.dart';
import 'engine_interface.dart';
import 'core_widget.dart';

import 'package:z6_theme/theme_selection_widget.dart';

class Z6IndianClassicBoard extends StatefulWidget {
  final int side;
  final String id;
  final bool both;
  const Z6IndianClassicBoard({
    super.key,
    required this.id,
    required this.side,
    required this.both,
  });

  @override
  State<Z6IndianClassicBoard> createState() => Z6IndianClassicBoardState();
}

class Z6IndianClassicBoardState extends State<Z6IndianClassicBoard> {
  late Z6IndianClassicEngine? _engine;
  late Z6IndianClassicSide side0;
  late Z6IndianClassicSide side1;
  bool started = false;
  bool ended = false;
  String winnername = "winner";
  int turn = -1;
  @override
  void initState() {
    super.initState();
    _engine = Z6IndianClassicInterface.get(widget.id);
    debugPrint('\@Z6IndianClassicBoardState -> \@initState');

    Z6IndianClassicInterface.observe(
      widget.id,
      Z6IndianClassicEvent.start,
      startme,
    );
    Z6IndianClassicInterface.observe(
      widget.id,
      Z6IndianClassicEvent.finish,
      gameOver,
    );

    side0 = Z6IndianClassicSide(
      id: widget.id,
      side: 0,
      active: widget.both ? true : (widget.side == 0),
    );
    side1 = Z6IndianClassicSide(
      id: widget.id,
      side: 1,
      active: widget.both ? true : (widget.side == 1),
    );
  }

  void gameOver() async {
    ended = true;
    debugPrint('\@Z6IndianClassicBoardState -> \@gameover');
    int winside = _engine?.winside ?? -1;
    if (winside >= 0) {
      winnername = (winside == 0)
          ? _engine?.player1 ?? "player 1"
          : _engine?.player2 ?? "player 2";
    }

    setState(() {});
  }

  // Called by Z6IndianClassicToss via its onTossComplete callback
  void startme() async {
    started = true;
    debugPrint('\@Z6IndianClassicBoardState -> \@start ');
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;

    return Scaffold(
      appBar: AppBar(
        title: Text("Z6 Indian Classic", style: theme.color?.text),
      ),
      drawer: Drawer(
        child: ListView(
          padding: EdgeInsets.zero,
          children: [
            DrawerHeader(
              decoration: BoxDecoration(
                color: theme.color?.fill ?? Colors.blue,
              ),
              child: Text(
                "Round: ${_engine?.currentRound ?? 0}",
                style: theme.color?.text,
              ),
            ),
            ExpansionTile(
              leading: const Icon(Icons.history),
              title: const Text('View History'),
              children: [
                for (String item in _engine?.history ?? [])
                  ListTile(leading: const Icon(Icons.add), title: Text(item)),
              ],
            ),
          ],
        ),
      ),
      body: SingleChildScrollView(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: !ended
              ? started
                    ? [Expanded(child: side0), Expanded(child: side1)]
                    : [SizedBox(width: 100, height: 100)]
              : [
                  Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      const Text(
                        "Game Ended",
                        style: TextStyle(
                          fontSize: 20,
                          fontWeight: FontWeight.bold,
                          color: Colors.redAccent,
                        ),
                      ),
                      Text(
                        "Winner: $winnername",
                        style: const TextStyle(
                          fontSize: 20,
                          fontWeight: FontWeight.bold,
                          color: Colors.redAccent,
                        ),
                      ),
                    ],
                  ),
                ],
        ),
      ),
    );
  }
}
