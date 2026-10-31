/// Project: Z6 Mancala World
/// File: z6_indian_classic.dart

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_theme/theme_template.dart';

import 'base_object.dart';

import 'package:z6_theme/theme_selection_widget.dart';

import 'engine.dart';
import 'engine_interface.dart';

class Z6IndianClassicPit extends StatelessWidget {
  final int index;
  final bool active;
  final int value;
  final bool a;
  final Function(int index)? onPitTap;

  const Z6IndianClassicPit({
    super.key,
    required this.index,
    required this.value,
    required this.active,
    required this.a,
    required this.onPitTap,
  });

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;

    return Stack(
      children: [
        Center(child: active ? theme.pit.normal(value) : theme.pit.inactive()),
        Positioned.fill(
          child: Material(
            color: Colors.transparent,
            child: InkWell(
              borderRadius: BorderRadius.circular(50),
              onTap: () {
                if (active && (onPitTap != null) && (value > 0)) {
                  onPitTap!(index);
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
  final bool a;

  Z6IndianClassicStore({super.key, required this.storeModel, required this.a});

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return Center(child: theme.pit.store(storeModel.value));
  }
}

class Z6IndianClassicSide extends StatefulWidget {
  final int side;
  final String id;
  final bool active;

  Z6IndianClassicSide({
    super.key,
    required this.id,
    required this.side,
    required this.active,
  });

  @override
  State<Z6IndianClassicSide> createState() => Z6IndianClassicSideState();
}

class Z6IndianClassicSideState extends State<Z6IndianClassicSide> {
  bool turn = false;
  late Z6IndianClassicEngine? _engine;
  late Z6MoveSummary _z6moveSummary;
  late Z6Store _store;
  final List<Z6Pit> _pits = [];
  bool onStoreChanged = false;
  bool onChange = false;
  bool _tapEnable = false;
  String playername = "Player-X";

  @override
  void initState() {
    super.initState();
    _engine = Z6IndianClassicInterface.get(widget.id);
    debugPrint(
      '\@Z6IndianClassicSideState -> \@initState: $widget.side $widget.active',
    );
    Z6IndianClassicInterface.observe(
      widget.id,
      Z6IndianClassicEvent.turn,
      myturn,
    );
    Z6IndianClassicInterface.observe(
      widget.id,
      Z6IndianClassicEvent.move,
      showTime,
    );
    playername = (widget.side == 0)
        ? _engine?.player1 ?? playername
        : _engine?.player2 ?? playername;
    _store = Z6Store(side: widget.side);
    _z6moveSummary = _engine?.getMoveSummary() ?? Z6MoveSummary();
    showTime();
  }

  bool _checkSideFail(Z6Action x) {
    return (x.side != widget.side);
  }

  void updateInit(Z6Action x) {
    debugPrint(
      '\@Z6IndianClassicSideState -> \@init: ${x.value} <- ${x.index}',
    );
    int index = int.tryParse(x.index) ?? -1;
    if (index >= 0) {
      Z6Pit pit = Z6Pit(
        index: index,
        side: x.side,
        value: x.value,
        active: true,
      );
      _pits.add(pit);
    }
  }

  void store(Z6Action x) {
    debugPrint('\@Z6IndianClassicSideState -> \@store: ${x.value}');
    if (x.index == Z6Action.store) {
      onStoreChanged = !onStoreChanged;
      _store.value = x.value;
    }
  }

  void roundup(Z6Action x) {
    int index = int.tryParse(x.index) ?? -1;
    if (index >= 0) {
      for (var pit in _pits) {
        if (pit.index == index) {
          pit.reset(x.value);
          break;
        }
      }
    }
  }

  void drop(Z6Action x) async {
    debugPrint(
      '\@Z6IndianClassicSideState -> \@capture: ${x.index} <-${x.value}',
    );
    int index = int.tryParse(x.index) ?? -1;
    if (index >= 0) {
      for (var pit in _pits) {
        if (pit.index == index) {
          pit.inc();
          break;
        }
      }
      setState(() {
        onChange = !onChange;
      });
    }
  }

  void scoop(Z6Action x) {
    debugPrint('\@Z6IndianClassicSideState -> \@scoop: ${x.index} ');
    int index = int.tryParse(x.index) ?? -1;
    if (index >= 0) {
      for (var pit in _pits) {
        if (pit.index == index) {
          pit.scoop();
          break;
        }
      }
      setState(() {
        onChange = !onChange;
      });
    }
  }

  void capture(Z6Action x) async {
    String by = "";
    if (x.act == Z6ActionType.capture) {
      by = "capture";
    } else if (x.act == Z6ActionType.capture2x) {
      by = "double capture";
    } else if (x.act == Z6ActionType.pasu) {
      by = "pasu - ${x.index}";
    } else if (x.act == Z6ActionType.capture0x) {
      by = "free - ${x.index}";
    }
    debugPrint('\@Z6IndianClassicSideState -> \@capture: ${x.value} / $by ');
  }

  void showTime() async {
    debugPrint('\@Z6IndianClassicSideState -> \@showTime: ');
    for (var x in _z6moveSummary.action) {
      if (_checkSideFail(x)) continue;
      switch (x.act) {
        case Z6ActionType.init:
          updateInit(x);
          break;
        case Z6ActionType.drop:
          drop(x);
          break;
        case Z6ActionType.scoop:
          scoop(x);
          break;
        case Z6ActionType.ghost:
        case Z6ActionType.roundup:
          roundup(x);
          break;
        case Z6ActionType.store:
          store(x);
          break;
        case Z6ActionType.capture2x:
        case Z6ActionType.pasu:
        case Z6ActionType.capture:
        case Z6ActionType.capture0x:
          capture(x);
          break;
        default:
          break;
      }
    }
    setState(() {
      onChange = !onChange;
    });
  }

  void myturn() async {
    isTurn();
    showTime();
  }

  void isTurn() {
    int now = _engine?.turn ?? -1;
    debugPrint('\@Z6IndianClassicSideState -> \@myturn: $now ');
    turn = (now == widget.side) ? true : false;
    if (turn) {
      _tapEnable = true;
    }
  }

  void onPitTapped(int index) {
    debugPrint(
      '\@Z6IndianClassicSideState -> \@onPitTapped: ${widget.side}, ${widget.active}, $index ',
    );
    if (_tapEnable && widget.active) {
      _tapEnable = false;
      Z6IndianClassicInterface.move(widget.id, widget.side, index);
    }
  }

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    isTurn();
    return Center(
      child: Container(
        padding: EdgeInsets.all(16),
        decoration: BoxDecoration(
          color: turn ? theme.color?.empty : theme.color?.fill,
          borderRadius: BorderRadius.circular(20),
          border: Border.all(color: Colors.brown[800]!, width: 4.0),
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            Text("Side $playername", style: theme.color?.text),
            for (var pit in _pits)
              Z6IndianClassicPit(
                active: pit.active,
                index: pit.index,
                value: pit.value,
                a: onChange,
                onPitTap: (index) {
                  onPitTapped(index);
                },
              ),
            Z6IndianClassicStore(storeModel: _store, a: onStoreChanged),
            Text("Side ${widget.side}", style: theme.color?.text),
          ],
        ),
      ),
    );
  }
}
