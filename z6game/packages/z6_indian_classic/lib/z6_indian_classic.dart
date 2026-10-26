/// Project: Z6 Mancala World
/// File: z6_indian_classic.dart

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_theme/theme_template.dart';
import 'base_object.dart';
import 'package:z6_theme/theme_selection_widget.dart';

class Z6IndianClassicPit extends StatefulWidget {
  final int index;
  final int initialValue;
  final int side;
  final bool initialActive;
  final Function(int pitIndex)? onPitTap;

  const Z6IndianClassicPit({
    super.key,
    required this.index,
    required this.initialValue,
    required this.side,
    required this.initialActive,
    this.onPitTap,
  });

  @override
  State<Z6IndianClassicPit> createState() => Z6IndianClassicPitState();
}

class Z6IndianClassicPitState extends State<Z6IndianClassicPit> {
  late final Z6Pit _pitModel;

  @override
  void initState() {
    super.initState();
    _pitModel = Z6Pit(
      index: widget.index,
      value: widget.initialValue,
      active: widget.initialActive,
      side: widget.side,
    );
  }

  void inc() {
    setState(() {
      _pitModel.inc();
    });
  }

  int scoop() {
    int scoopedCount = -1;
    setState(() {
      scoopedCount = _pitModel.scoop();
    });
    return scoopedCount;
  }

  void updatePit({int? newValue, bool? newActive}) {
    setState(() {
      if (newValue != null) _pitModel.value = newValue;
      if (newActive != null) _pitModel.active = newActive;
    });
  }

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;

    return Stack(
      children: [
        Center(
          child: _pitModel.active 
              ? theme.pit.normal(_pitModel.value)  
              : theme.pit.inactive(),
        ),
        Positioned.fill(
          child: Material(
            color: Colors.transparent,
            child: InkWell(
              borderRadius: BorderRadius.circular(50),
              onTap: () {
                if (widget.onPitTap != null) {
                  widget.onPitTap!(_pitModel.index);
                }
              },
            ),
          ),
        ),
      ],
    );
  }
}

class Z6IndianClassicStore extends StatelessWidget {
  final Z6Store storeModel;

  const Z6IndianClassicStore({super.key, required this.storeModel});

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return Center(
      child: theme.pit.store(storeModel.value) ?? Container(),
    );
  }
}

class Z6IndianClassicSide extends StatelessWidget {
  final Z6SideModel sideModel;
  final bool isTurn;
  final Function(int index) onPitTapped;

  const Z6IndianClassicSide({
    super.key,
    required this.sideModel,
    required this.isTurn,
    required this.onPitTapped,
  });

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;

    return Scaffold(
      appBar: AppBar(
        title: Text("Side ${sideModel.side}", style: theme.color?.text ),
      ),
      body: Container(
        padding: const EdgeInsets.all(16),
        decoration: BoxDecoration(
          color: isTurn ? theme.color?.empty : theme.color?.fill,          
          borderRadius: BorderRadius.circular(20), 
          border: Border.all(
            color: Colors.brown[800]!,     
            width: 4.0,                    
          ),
        ),  
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            for (var pit in sideModel.pits)
              Z6IndianClassicPit(
                index: pit.index,
                initialValue: pit.value,
                side: pit.side,
                initialActive: pit.active,
                onPitTap: (index) {
                  if (isTurn && pit.value > 0) {
                    onPitTapped(index);
                  }
                },
              ),
            Z6IndianClassicStore(storeModel: sideModel.store),
          ],
        ),
      ),
    );
  }
}
