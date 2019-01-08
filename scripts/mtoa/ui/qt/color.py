
class Color(object):
    """base Color class for translating byte colors to qss strings
    Color(255,255,255) translates to "rgb(255,255,255)"
    """

    def __init__(self,r,g,b,a=255):
        self.color = []
        self.color.extend([r,g,b,a])

    @property
    def red(self): return self.color[0]
    @property
    def green(self):return self.color[1]
    @property
    def blue(self):return self.color[2]
    @property
    def rgb(self):  return "rgb("+",".join([str(item) for item in self.color[:-1] ]) + ")"
    @property
    def rgba(self): return "rgba("+",".join([str(item) for item in self.color]) + ")"

    def __repr__(self):
        return str(self.color)

    def __str__(self):
        return self.rgb
