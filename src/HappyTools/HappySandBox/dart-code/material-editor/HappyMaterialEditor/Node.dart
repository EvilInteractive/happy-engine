
class Connecter
{
  static const int TEXT_MARGE = 4;
  static const int SIZE = 8;
  
  String m_Name;
  int m_Index;
  var m_Parent;
  bool m_IsInput;
  
  Connecter(this.m_Name)
  {
  }
    
  void draw(CanvasRenderingContext2D context)
  {
    double x = m_Parent.GetXPosConnecter(this);
    double y = m_Parent.GetYPosConnecter(this);
    
    context.setShadow(0, 0, 0, 0, 0);
    context.setFillColor(0.1, 0.1, 0.1, 1);
    context.fillRect(x - SIZE / 2, y - SIZE / 2, SIZE, SIZE);
    if (m_IsInput == false)
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
  
  static const int MARGIN_TOP = 8;
  static const int MARGIN_BOTTOM = 8;
  static const int MARGIN_LEFT = 8;
  static const int MARGIN_RIGHT = 8;
  
  vec2 m_Position;
  List<Connecter> inputs;
  List<Connecter> outputs;
  bool m_IsSelected = false;
  
  Node(this.m_Position):
    inputs = new List<Connecter>(),
    outputs = new List<Connecter>()
  {
  }
  
  bool collisionCheck(double x, double y)
  {
    return x > m_Position.x - WIDTH/2 && x < m_Position.x + WIDTH/2 &&
           y > m_Position.y - HEIGHT/2 && y < m_Position.y + HEIGHT/2;
  }
  
  void addInput(Connecter input)
  {
    input.m_IsInput = true;
    input.m_Index = inputs.length;
    input.m_Parent = this;
    inputs.add(input);
  }
  void addOutput(Connecter output)
  {
    output.m_IsInput = false;
    output.m_Index = outputs.length;
    output.m_Parent = this;
    outputs.add(output);
  }
  
  double GetXPosConnecter(Connecter c)
  {
    if (c.m_IsInput)
      return m_Position.x - WIDTH/2 + WIDTH;
    else
      return m_Position.x - WIDTH/2;
  }
  double GetYPosConnecter(Connecter c)
  {
    int count = c.m_IsInput? inputs.length : outputs.length;
    double yInputOffset = (HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / (count + 1);
    return m_Position.y - HEIGHT/2 + MARGIN_TOP + yInputOffset * (c.m_Index + 1);
  }
  
  void draw(CanvasRenderingContext2D context)
  {
    if (m_IsSelected)
    {
      context.setShadow(4, 4, 8, 0, 0.5);
      context.setFillColor(0.5, 0.3, 1, 1);
    }
    else
    {
      context.setShadow(2, 2, 8, 0, 0.5);
      context.setFillColor(0.5, 0.1, 1, 1);
    }
    context.fillRect(m_Position.x - WIDTH/2, m_Position.y - HEIGHT/2, WIDTH, HEIGHT);
    
    inputs.forEach((c) => c.draw(context));
    outputs.forEach((c) => c.draw(context));
  }
}
