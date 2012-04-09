import re

from pygments.scanner import Scanner
from pygments.lexer import Lexer, RegexLexer, include, bygroups, using, \
                           this, combined
from pygments.util import get_bool_opt, get_list_opt
from pygments.token import *

# backwards compatibility
from pygments.lexers.functional import OcamlLexer

__all__ = ['ArnoldLexer', 'MetadataLexer']


class ArnoldLexer(RegexLexer):
    name = 'Arnold'
    aliases = ['arnold','ass']
    filenames = ['*.ass']

    tokens = {
        'root': [
            (r'#.*\n', Comment),
            (r'\n', Text),
            (r'\s+', Text),
            ('{', Text, 'node'),
            (r'[a-zA-Z_][a-zA-Z0-9_]*', Name.Class),
        ],
        'node': [
            (r'}', Text, '#pop'),
            (r'###.*\n', Comment),
            (r'\n', Text),
            (r'\s+', Text),
            (r'[a-zA-Z_][a-zA-Z0-9_]*', Keyword, 'attr'),
        ],
        'attr': [
            (r'}', Text, 'root'),
            (r'\n', Text),
            (r'\s+', Text),
            (r'"[^"]*"', String, '#pop'),
            (r'[a-zA-Z_][a-zA-Z0-9_]*', Text, '#pop'),
            (r'\d+\s+\d+\s+\d+\s+\d+\s+'
             r'\d+\s+\d+\s+\d+\s+\d+\s+'
             r'\d+\s+\d+\s+\d+\s+\d+\s+'
             r'\d+\s+\d+\s+\d+\s+\d+', Number.Integer, '#pop'),
            (r'\d+\s+\d+\s+\d+\s+\d+', Number.Integer, '#pop'),
            (r'\d+\s+\d+\s+\d+', Number.Integer, '#pop'),
            (r'\d+', Number.Integer, '#pop'),
            (r'(\d+\.\d*|\.\d+|\d+)[eE][+-]?\d+[LlUu]*', Number.Float, '#pop'),
        ]
    }
    

class MetadataLexer(RegexLexer):
    name = 'ArnoldMtd'
    aliases = ['mtd']
    filenames = ['*.mtd']

    tokens = {
        'root': [
            (r'#.*\n', Comment),
            (r'\n', Text),
            (r'\s+', Text),
            (r'\[node .+\]$', Keyword),
            (r'\[attr .+\]$', Keyword),
            (r'([^\s]+)(\s*)([^\s]+)(\s*)([^\s].*)$',
             bygroups(Name.Attribute, Text, Operator, Text, String))
            
        ]
    }