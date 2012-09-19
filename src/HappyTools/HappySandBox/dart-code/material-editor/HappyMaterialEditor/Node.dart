#import('dart:html');

class Connecter
{
  static const int TEXT_MARGE = 4;
  static const int SIZE = 4;
  
  String m_Name;
    
  void draw(CanvasRenderingContext2D context, double x, double y, bool leftAlign)
  {
    context.fillRect(x, y, SIZE, SIZE);
    if (leftAlign)
    {
      context.textAlign = "left";
      context.fillText(m_Name, x + SIZE + TEXT_MARGE, y);
    }
    else
    {
      context.textAlign = "right";
      context.fillText(m_Name, x - TEXT_MARGE, y);
    }
  }
}

class Node 
{
  static const int WIDTH  = 128;
  static const int HEIGHT = 96;
  
  double m_X, m_Y;
  List<Connecter> inputs;
  List<Connecter> outputs;
  
  Node(this.m_X, this.m_Y)
  {
  }
  
  void addInput(Connecter input)
  {
    inputs.add(input);
  }
  void addOutput(Connecter output)
  {
    outputs.add(output);
  }
  
  void draw(CanvasRenderingContext2D context)
  {
    // TODO: set Node style
    context.fillRect(m_X, m_Y, WIDTH, HEIGHT);
    
    double yInputOffset = (HEIGHT - style.marginTop - style.marginBottom) / inputs.length;
    for (var i = 0; i < inputs.length; ++i)
    {
      double x = m_X + WIDTH;
      double y = m_Y + style.marginTop + yInputOffset * i;
      inputs[i].draw(context, x, y);
    }
  }
}
