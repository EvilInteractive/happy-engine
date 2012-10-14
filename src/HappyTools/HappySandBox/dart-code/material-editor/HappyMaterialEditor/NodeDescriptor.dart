
class ValueTypes
{
  static const String Bool="Bool";
  static const String Float="Float";
  static const String Float2="Float2";
  static const String Float3="Float3";
  static const String Float4="Float4";
  static const String Texture1D="Texture1D";
  static const String Texture2D="Texture2D";
  static const String Texture3D="Texture3D";
  static const String TextureCube="TextureCube";
  
  static String ShortToFull(String short)
  {
    switch (short)
    {
      case "b":  return Bool;
      case "f":  return Float;
      case "f2": return Float2;
      case "f3": return Float3;
      case "f4": return Float4;
      case "t1": return Texture1D;
      case "t2": return Texture2D;
      case "t3": return Texture3D;
      case "tc": return TextureCube;
    }
  }
  
}

class ConnectionOverload
{
  List<String> inputsTypes;
  List<String> outputsTypes;
  
  ConnectionOverload():
    inputsTypes = new List<String>(),
    outputsTypes = new List<String>()
  {}
}
class ParameterDescriptor
{
  String name;
  String typeID;
  ParameterDescriptor(this.name, this.typeID) {}
}
class NodeDescriptor 
{
  String name;
  int typeID;

  List<String> inputNames;
  List<String> outputNames;
  List<ConnectionOverload> overloads;
  List<ParameterDescriptor> parameters;
  String shortcut = "";
  
  NodeDescriptor(this.name, this.typeID):
        inputNames = new List<String>(),
        outputNames = new List<String>(),
        overloads = new List<ConnectionOverload>()
  {
  }
  
  void AddFormatedNames(String connections)
  {
    List<String> inout = connections.split(":");
    List<String> splitInputs = inout[0].split(",");
    inputNames.addAll(splitInputs.filter(f(e) => e != ""));
    List<String> splitOutputs = inout[1].split(",");
    outputNames.addAll(splitOutputs.filter(f(e) => e != ""));
  }
  void AddFormatedOverload(String connections)
  {
    List<String> inout = connections.split(":");
    ConnectionOverload overload = new ConnectionOverload();
    
    List<String> splitInputs = inout[0].split(",").filter(f(e) => e != "");    
    List<String> splitOutputs = inout[1].split(",").filter(f(e) => e != "");
    
    overload.inputsTypes.addAll(splitInputs);
    overload.outputsTypes.addAll(splitOutputs);
    
    overloads.add(overload);
  }
}


