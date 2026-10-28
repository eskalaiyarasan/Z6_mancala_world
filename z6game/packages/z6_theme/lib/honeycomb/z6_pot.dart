
import 'package:flutter/material.dart';

class IndianPot extends StatelessWidget {
  final Size size;
  final String ownerName;
  final String contentLabel;
  final Color potColor;
  final Color accentColor;
  final Color designColor;
  final double bellyFatness;
  final double neckWidthFactor;
  final double rimWidthFactor;
  final bool showPatterns;

  IndianPot({
    super.key,
    required this.size,
    this.ownerName = "",
    this.contentLabel = "",
    this.potColor = const Color(0xFFB85A38),
    this.accentColor = const Color(0xFF5C2C16),
    this.designColor = const Color(0xFFE6C687),
    this.bellyFatness = 0.95,
    this.neckWidthFactor = 0.38,
    this.rimWidthFactor = 0.55,
    this.showPatterns = true,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: size.width,
      height: size.height,
      child: Stack(
        alignment: Alignment.center,
        children: [
          // 1. Vector Clay Pot Canvas Base
          Positioned.fill(
            child: CustomPaint(
              painter: ConfigurableIndianPotPainter(
                potColor: potColor,
                accentColor: accentColor,
                designColor: designColor,
                bellyFatness: bellyFatness,
                neckWidthFactor: neckWidthFactor,
                rimWidthFactor: rimWidthFactor,
                showPatterns: showPatterns,
              ),
            ),
          ),
          
          // 2. Neck Layer: Pot's Owner Name
          Positioned(
            top: size.height * 0.09, 
            child: SizedBox(
              width: size.width * (neckWidthFactor * 1.2), 
              child: Text(
                ownerName.toUpperCase(),
                textAlign: TextAlign.center,
                maxLines: 1,
                overflow: TextOverflow.ellipsis,
                style: TextStyle(
                  color: const Color(0xFF5C2C16), 
                  fontSize: (size.height * 0.05).clamp(8.0, 13.0), 
                  fontWeight: FontWeight.w900,
                  letterSpacing: 1.0,
                ),
              ),
            ),
          ),

          // 3. Belly Layer: Storage Contents Label
          Positioned(
            top: size.height * 0.45, 
            child: SizedBox(
              width: size.width * 0.65, 
              child: Text(
                contentLabel,
                textAlign: TextAlign.center,
                maxLines: 1,
                overflow: TextOverflow.ellipsis,
                style: TextStyle(
                  color: const Color(0xFFE8E8E8), 
                  fontSize: (size.height * 0.1).clamp(14.0, 24.0), 
                  fontWeight: FontWeight.bold,
                  shadows: const [
                    Shadow(
                      color: Colors.black38,
                      offset: Offset(1, 2),
                      blurRadius: 3,
                    ),
                  ],
                  letterSpacing: 2.0,
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class ConfigurableIndianPotPainter extends CustomPainter {
  final Color potColor;
  final Color accentColor;
  final Color designColor;
  final double bellyFatness;
  final double neckWidthFactor;
  final double rimWidthFactor;
  final bool showPatterns;

  ConfigurableIndianPotPainter({
    required this.potColor,
    required this.accentColor,
    required this.designColor,
    required this.bellyFatness,
    required this.neckWidthFactor,
    required this.rimWidthFactor,
    required this.showPatterns,
  });

  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint()
      ..color = potColor
      ..style = PaintingStyle.fill
      ..isAntiAlias = true;

    final w = size.width;
    final h = size.height;

    double rimWidth = w * rimWidthFactor;
    double neckWidth = w * neckWidthFactor;
    double rimHeight = h * 0.08;
    double neckHeight = h * 0.18;

    double centerX = w / 2;
    double rimLeft = centerX - (rimWidth / 2);
    double rimRight = centerX + (rimWidth / 2);
    double neckLeft = centerX - (neckWidth / 2);
    double neckRight = centerX + (neckWidth / 2);

    // --- DRAW MAIN SILHOUETTE ---
    final path = Path();
    path.moveTo(rimLeft, rimHeight);
    path.quadraticBezierTo(rimLeft - 5, 0, rimLeft + 15, 0); 
    path.lineTo(rimRight - 15, 0);                           
    path.quadraticBezierTo(rimRight + 5, 0, rimRight, rimHeight); 
    path.quadraticBezierTo(centerX + (w * (rimWidthFactor - neckWidthFactor)), neckHeight * 0.7, neckRight, neckHeight);

    double bellyMaxY = h * 0.62; 
    path.cubicTo(
      w * (0.5 + (bellyFatness * 0.45)), neckHeight + (h * 0.1),
      w * (0.5 + (bellyFatness * 0.55)), bellyMaxY - (h * 0.1),
      centerX + (w * 0.14), h - 8,
    );

    path.quadraticBezierTo(centerX, h, centerX - (w * 0.14), h - 8);

    path.cubicTo(
      w * (0.5 - (bellyFatness * 0.55)), bellyMaxY - (h * 0.1),
      w * (0.5 - (bellyFatness * 0.45)), neckHeight + (h * 0.1),
      neckLeft, neckHeight,
  );
  path.quadraticBezierTo(centerX - (w * (rimWidthFactor - neckWidthFactor)), neckHeight * 0.7, rimLeft, rimHeight);
  path.close();
  canvas.drawPath(path, paint);
  // --- DECORATIVE LAYERS ---
  final linePaint = Paint()
    ..color = accentColor
    ..style = PaintingStyle.stroke
    ..strokeWidth = 3.5;
  final neckAccent = Path();
  neckAccent.moveTo(neckLeft - 5, neckHeight);
  neckAccent.quadraticBezierTo(centerX, neckHeight + 8, neckRight + 5, neckHeight);
  canvas.drawPath(neckAccent, linePaint);

  if (showPatterns) {
    final designPaint = Paint()
      ..color = designColor
      ..style = PaintingStyle.stroke
      ..strokeWidth = 2.0;
    final bodyAccent1 = Path();
    bodyAccent1.moveTo(w * 0.09, h * 0.42);
    bodyAccent1.quadraticBezierTo(centerX, (h * 0.42) + 20, w * 0.91, h * 0.42);
    canvas.drawPath(bodyAccent1, linePaint);
    final bodyAccent2 = Path();
    bodyAccent2.moveTo(w * 0.06, h * 0.65);
    bodyAccent2.quadraticBezierTo(centerX, (h * 0.65) + 24, w * 0.94, h * 0.65);
    canvas.drawPath(bodyAccent2, linePaint);

    // Ethnic Zig-Zag Chevron Pattern inside the neck collar
    final zigZagPath = Path();
    double startX = neckLeft - 2;
    double endX = neckRight + 2;
    double currentX = startX;
    double step = (endX - startX) / 10;
    bool up = true;
    zigZagPath.moveTo(startX, neckHeight - 4);
    while (currentX < endX) {
      currentX += step;
      double currentY = up ? (neckHeight - 14) : (neckHeight - 4);
      zigZagPath.lineTo(currentX, currentY);
      up = !up;
    }
    canvas.drawPath(zigZagPath, designPaint);
    // Traditional decorative tribal dotting matrix
    final dotPaint = Paint()
      ..color = designColor
      ..style = PaintingStyle.fill;
    for (int i = 1; i <= 6; i++) {
      double dotX = neckLeft + (i * ((neckRight - neckLeft) / 7));
      canvas.drawCircle(Offset(dotX, neckHeight + 16), 3.0, dotPaint);
    }
  }
}

  @override
  bool shouldRepaint(covariant ConfigurableIndianPotPainter oldDelegate) {
    return oldDelegate.potColor != potColor ||
      oldDelegate.accentColor != accentColor ||
      oldDelegate.designColor != designColor ||
      oldDelegate.bellyFatness != bellyFatness ||
      oldDelegate.neckWidthFactor != neckWidthFactor ||
      oldDelegate.rimWidthFactor != rimWidthFactor ||
      oldDelegate.showPatterns != showPatterns;
  }
}
