/// Project: Z6 Mancala World
/// File: z6_indian_classic_ai.dart

import 'package:flutter/material.dart';

import 'board.dart';
import 'toss.dart';
import 'opponent.dart';
import 'engine_interface.dart';

class Z6IndianClassicAI extends StatefulWidget {
  final int level;
  final String currentUserName;

  const Z6IndianClassicAI({
    super.key,
    required this.level,
    this.currentUserName = "Kalai",
  });

  @override
  State<Z6IndianClassicAI> createState() => _Z6IndianClassicAIState();
}

class _Z6IndianClassicAIState extends State<Z6IndianClassicAI> {
  late String id;
  @override
  void initState() {
    super.initState();
    id = Z6IndianClassicInterface.create(
      widget.level,
      widget.currentUserName,
      "AI",
    );
    WidgetsBinding.instance.addPostFrameCallback((_) => _showTossDialog());
  }

  void _showTossDialog() {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => Z6IndianClassicToss(id: id),
    );
  }

  @override
  void dispose() {
    Z6IndianClassicInterface.destroy(id);
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          Z6IndianClassicBoard(id: id, side: 0, both: false),
          // Embedded AI Opponent Listener interacting via GlobalKey hooks
          Z6IndianClassicOpponent(id: id, side: 1),
        ],
      ),
    );
  }
}
