var jDoc = {
    "ChainLoader" : {
        "className" : "ChainLoader",
        "desc" : "Static class used for loading files into TChains. Can be used to load an entire directory, or files from a filelist.lis",
        "methods" : [
            {
                "desc" : "Loads .root tree files into a TChain",
                "name" : "static void load( TChain * chain, string dir, int maxFiles = -1 )",
                "paramDesc" : [
                    "A pointer to an initialize TChain ready to add files",
                    "The path to the directory containing .root files",
                    "Optional: maximum number of files to load into chain"
                ],
                "params" : [
                    "chain",
                    "dir",
                    "maxFiles"
                ],
                "returns" : []
            },
            {
                "desc" : "Loads .root tree files into a TChain - used in parallel jobs",
                "name" : "static void loadList( TChain * chain, string listFile, int maxFiles = -1 )",
                "paramDesc" : [
                    "A pointer to an initialize TChain ready to add files",
                    "filename of a file containg the list of .root files to load",
                    "Optional: maximum number of files to load into chain"
                ],
                "params" : [
                    "chain",
                    "listFile",
                    "maxFiles"
                ],
                "returns" : []
            }
        ],
        "properties" : []
    },
    "ConfigPoint" : {
        "className" : "ConfigPoint",
        "desc" : "A point that can easily be initialized from a config file",
        "methods" : [
            {
                "desc" : "Creates a point object from the project config. If values are not in the config the variables are initialized and set to the default",
                "name" : "ConfigPoint( XmlConfig * cfg, string nodePath, double dX = 0, double dY = 0, string attrX = \":x\", string attrY = \":y\")",
                "paramDesc" : [
                    "Project config",
                    "Path to root node in config",
                    "Optional: Default x value",
                    "Optional: Default y value",
                    "Optional: Attribute to read from for x value",
                    "Optional: Attribute to read from for y value"
                ],
                "params" : [
                    "cfg",
                    "nodePath",
                    "dX",
                    "dY",
                    "attrX",
                    "attrY"
                ],
                "returns" : []
            },
            {
                "desc" : "Example: \"( 0.0, 14.0 )\" ",
                "name" : "string toString()",
                "paramDesc" : [],
                "params" : [],
                "returns" : [
                    "String representation of point"
                ]
            }
        ],
        "properties" : [
            {
                "desc" : "",
                "name" : "double x"
            },
            {
                "desc" : "",
                "name" : "double y"
            }
        ]
    },
    "ConfigRange" : {
        "className" : "ConfigRange",
        "desc" : "A range that can easily be initialized from a config file",
        "methods" : [
            {
                "desc" : "Creates a range object from the project config. If values are not in the config the variables are initialized and set to the default",
                "name" : "ConfigRange( XmlConfig * cfg, string np, double dMin = 0, double dMax = 0, string attrMin = \":min\", string attrMax = \":max\")",
                "paramDesc" : [
                    "Project config",
                    "Path to root node in config",
                    "Optional: Default min value",
                    "Optional: Default max value",
                    "Optional: Attribute to read from for min value",
                    "Optional: Attribute to read from for max value"
                ],
                "params" : [
                    "cfg",
                    "nodePath",
                    "dMin",
                    "dMax",
                    "attrMin",
                    "attrMax"
                ],
                "returns" : []
            },
            {
                "desc" : "Example: \"( 0.0 -> 14.0 )\" ",
                "name" : "string toString()",
                "paramDesc" : [],
                "params" : [],
                "returns" : [
                    "String representation of the range"
                ]
            }
        ],
        "properties" : [
            {
                "desc" : "Minimum value in range",
                "name" : "double min"
            },
            {
                "desc" : "Maximum value in range",
                "name" : "double max"
            }
        ]
    },
    "HistoAnalyzer" : {
        "className" : "HistoAnalyzer",
        "desc" : "Base class for creating histogram analysis projects",
        "methods" : [
            {
                "desc" : "",
                "name" : "HistoAnalyzer( XmlConfig * config, string nodePath )",
                "paramDesc" : [
                    "Project config object",
                    "The node path containing configuration parameters"
                ],
                "params" : [
                    "config",
                    "nodePath"
                ],
                "returns" : []
            },
            {
                "desc" : "The maker function for publicly starting the job",
                "name" : "virtual void make()",
                "paramDesc" : [],
                "params" : [],
                "returns" : []
            }
        ],
        "properties" : [
            {
                "desc" : "The logging object for the job",
                "name" : "Logger *logger"
            },
            {
                "desc" : "",
                "name" : "XmlConfig *cfg"
            },
            {
                "desc" : "",
                "name" : "string nodePath"
            },
            {
                "desc" : "",
                "name" : "HistoBook *book"
            },
            {
                "desc" : "",
                "name" : "Reporter *reporter"
            },
            {
                "desc" : "Input file with histogram data",
                "name" : "TFile *inFile"
            }
        ]
    },
    "HistoBins" : {
        "className" : "HistoBins",
        "desc" : "Container and utility class for histogram style binning",
        "methods" : [
            {
                "desc" : "Divides the range high - low into a fixed number of bins from low to high",
                "name" : "static vector<double> makeNBins( int nBins, double low, double high )",
                "paramDesc" : [
                    "Number of Bins",
                    "Lower edge of first bin",
                    "Upper edge of last bin"
                ],
                "params" : [
                    "int nBins",
                    "double low",
                    "double high"
                ],
                "returns" : [
                    "vector of bin edges"
                ]
            },
            {
                "desc" : "Divides the range high - low into a fixed number of bins from low to high",
                "name" : "static vector<double> makeFixedWidthBins( double binWidth, double low, double high )",
                "paramDesc" : [
                    "Nominal width of each bin",
                    "Lower edge of first bin",
                    "Upper edge of last bin"
                ],
                "params" : [
                    "double binWidth",
                    "double low",
                    "double high"
                ],
                "returns" : [
                    "vector of bin edges"
                ]
            },
            {
                "desc" : "Finds the bin corresponding to a value in the given bins",
                "name" : "static int findBin( vector<double> &bins, double val )",
                "paramDesc" : [
                    "Bins to search in",
                    "Search value"
                ],
                "params" : [
                    "&bins",
                    "val"
                ],
                "returns" : [
                    "-1 : Underflow",
                    "-2 : Overflow",
                    "-3 : Not Found",
                    "Otherwise the zero-indexed bin is returned"
                ]
            },
            {
                "desc" : "Finds the bin corresponding to a value in the object bins",
                "name" : "int findBin( double val )",
                "paramDesc" : [
                    "Search value"
                ],
                "params" : [
                    "val"
                ],
                "returns" : [
                    "-1 : Underflow",
                    "-2 : Overflow",
                    "-3 : Not Found",
                    "Otherwise the zero-indexed bin is returned"
                ]
            },
            {
                "name" : "int length()",
                "returns" : [
                    "Length of underlying vector containing the bin edges"
                ]
            },
            {
                "name" : "int size()",
                "returns" : [
                    "Size of underlying vector containing the bin edges"
                ]
            },
            {
                "name" : "int nBins()",
                "returns" : [
                    "Number of bins stored in the underlying vector of bin edges. Equal to size() - 1."
                ]
            },
            {
                "desc" : "Creates bins with a nominal bin width. The final bin may be smaller than the nominal size if width does not evenly divide the range",
                "name" : "HistoBins( double min, double max, double width )",
                "paramDesc" : [
                    "Bin minimum value",
                    "Bin maximum value",
                    "Nominal bin width"
                ],
                "params" : [
                    "min",
                    "max",
                    "width"
                ],
                "returns" : []
            },
            {
                "desc" : "Creates histogram bins from a config node. Used heavily by HistoBook for config based histogram creation. Attribute modifier is used to select, x, y, etc. from the node.",
                "name" : "HistoBins( XmlConfig * config, string nodePath, string mod = \"\" )",
                "paramDesc" : [
                    "Project config",
                    "Path to configuration node",
                    "Optional: Attribute modifier"
                ],
                "params" : [
                    "config",
                    "nodePath",
                    "mod"
                ],
                "returns" : []
            },
            {
                "desc" : "Gets a bin edge from the underlying vector of bin edges",
                "name" : "double& operator[] ( const int nIndex )",
                "paramDesc" : [
                    "Index to retrieve from the underlying vector of bin edges"
                ],
                "params" : [
                    "nIndex"
                ],
                "returns" : [
                    "Bin edge indexed by nIndex"
                ]
            },
            {
                "desc" : "",
                "name" : "string toString ()",
                "paramDesc" : [],
                "params" : [],
                "returns" : [
                    "String representation of bins"
                ]
            }
        ],
        "properties" : [
            {
                "desc" : "Vector containing the bin edges",
                "name" : "vector< double > bins"
            },
            {
                "desc" : "Nominal width of bins if fixed number or fixed width is used - not set for variable bin edges",
                "name" : "double width"
            },
            {
                "desc" : "",
                "name" : "double min"
            },
            {
                "desc" : "",
                "name" : "double max"
            }
        ]
    },
    "Logger" : {
        "className" : "Logger",
        "desc" : "An all purpose logging utility with log level functionality. Meant to be used in modular projects. Multpile Logger instances can be used with different log levels.",
        "methods" : [
            {
                "name" : "inline static int getGlobalLogLevel()",
                "returns" : [
                    "Global log level"
                ]
            },
            {
                "desc" : "Sets the global log level",
                "name" : "inline static void setGlobalLogLevel( int ll )",
                "paramDesc" : [
                    "Log Level"
                ],
                "params" : [
                    "ll"
                ],
                "returns" : []
            },
            {
                "desc" : "Creates a logger instance with the given properties",
                "name" : "Logger( int ll, string classSpace, ostream* os )",
                "paramDesc" : [
                    "Log Level",
                    "Class name to prepend to messages",
                    "Output stream"
                ],
                "params" : [
                    "ll",
                    "classSpace",
                    "os"
                ],
                "returns" : []
            },
            {
                "desc" : "Default constructor - uses the global log level that can be set with static method",
                "name" : "Logger( int ll = Logger::getGlobalLogLevel(), string classSpace = \"\" )",
                "paramDesc" : [
                    "Optional: log level set to global log level",
                    "Optional: Class name to prepend messages with"
                ],
                "params" : [
                    "ll",
                    "classSpace"
                ],
                "returns" : []
            },
            {
                "name" : "int getLogLevel()",
                "returns" : [
                    "Current log level"
                ]
            },
            {
                "name" : "void setLogLevel( int ll )",
                "paramDesc" : [
                    "new log level to use"
                ],
                "params" : [
                    "ll"
                ],
                "returns" : []
            },
            {
                "desc" : "Shows all messages below llWarn",
                "name" : "ostream & warn( string functionName = \"\" )",
                "paramDesc" : [
                    "Calling function name to be prepended to message"
                ],
                "params" : [
                    "functionName"
                ],
                "returns" : [
                    "An output stream for writing messages"
                ]
            },
            {
                "desc" : "Shows all messages below llError",
                "name" : "ostream & error( string functionName = \"\" )",
                "paramDesc" : [
                    "Calling function name to be prepended to message"
                ],
                "params" : [
                    "functionName"
                ],
                "returns" : [
                    "An output stream for writing messages"
                ]
            },
            {
                "desc" : "Shows all messages below llInfo",
                "name" : "ostream & info( string functionName = \"\" )",
                "paramDesc" : [
                    "Calling function name to be prepended to message"
                ],
                "params" : [
                    "functionName"
                ],
                "returns" : [
                    "An output stream for writing messages"
                ]
            },
            {
                "desc" : "Ca be one of [ info, warning, error, all, none ] default is all",
                "name" : "static int logLevelFromString( string ll )",
                "paramDesc" : [
                    "String representation of log level"
                ],
                "params" : [
                    "ll"
                ],
                "returns" : [
                    "Integer representation of log level"
                ]
            }
        ],
        "properties" : [
            {
                "desc" : "Show all log messages",
                "name" : "static const int llAll = 40"
            },
            {
                "desc" : "Show info and below",
                "name" : "static const int llInfo = 30"
            },
            {
                "desc" : "Show warnings and below",
                "name" : "static const int llWarn = 20"
            },
            {
                "desc" : "Show errors and below",
                "name" : "static const int llError = 10"
            },
            {
                "desc" : "Show nothing",
                "name" : "static const int llNone = 1"
            },
            {
                "desc" : "The default log level",
                "name" : "static const int llDefault = llWarn"
            },
            {
                "desc" : "The global log level used if not set otherwise",
                "name" : "static int llGlobal"
            }
        ]
    },
    "Reporter" : {
        "className" : "Reporter",
        "desc" : "A small utility class for creating PDF reports from ROOT canvas objects",
        "methods" : [
            {
                "desc" : "",
                "name" : "Reporter( string filename, int w = 791, int h = 1024 )",
                "paramDesc" : [
                    "Output filename",
                    "width of pdf page",
                    "height of page"
                ],
                "params" : [
                    "filename",
                    "w",
                    "h"
                ],
                "returns" : []
            },
            {
                "desc" : "",
                "name" : "Reporter( XmlConfig * config, string nodePath, string prefix = \"\" )",
                "paramDesc" : [
                    "Project config",
                    "Path to config node containing params",
                    "prefix for parallel jobs - prepended"
                ],
                "params" : [
                    "config",
                    "nodePath",
                    "prefix"
                ],
                "returns" : []
            },
            {
                "desc" : "Creates a new page with sub-divided pads",
                "name" : "void newPage( int dx = 1, int dy = 1)",
                "paramDesc" : [
                    "Optional: Number of divisions in x direction",
                    "Optional: Number of divisions in y direction"
                ],
                "params" : [
                    "dx",
                    "dy"
                ],
                "returns" : []
            },
            {
                "desc" : "The 0 pad is the entire page, individual pads start a 1",
                "name" : "void cd( int pad )",
                "paramDesc" : [
                    "The pad to set as the current"
                ],
                "params" : [
                    "pad"
                ],
                "returns" : []
            },
            {
                "desc" : "Sets the padX, padY as the current pad. (1, 1) is the top left pad",
                "name" : "void cd( int padX, int padY)",
                "paramDesc" : [
                    "X coord of pad",
                    "Y coord of pad"
                ],
                "params" : [
                    "padX",
                    "padY"
                ],
                "returns" : []
            },
            {
                "desc" : "Pushes to the next pad or next page",
                "name" : "void next()"
            },
            {
                "desc" : "To add to the main report leave name off. Otherwise a single page PDF will be made with the given name",
                "name" : "void savePage( string name = \"\" )",
                "paramDesc" : [
                    "Optional: output filename"
                ],
                "params" : [
                    "name"
                ]
            },
            {
                "desc" : "Saves an image to the given filename. Must have suffix to determine format, ie .png, .jpg etc.",
                "name" : "void saveImage( string name )",
                "paramDesc" : [
                    "Output filename"
                ],
                "params" : [
                    "name"
                ]
            }
        ],
        "properties" : []
    },
    "TreeAnalyzer" : {
        "className" : "TreeAnalyzer",
        "desc" : "",
        "methods" : [
            {
                "desc" : "",
                "name" : "TreeAnalyzer( XmlConfig * config, string nodePath, string fileList =\"\", string jobPrefix =\"\")",
                "paramDesc" : [
                    "Project's config object",
                    "The node path containing configuration parameters",
                    "Optional: File list for parallel jobs",
                    "Optional: Job prefix for parallel jobs. Will be prepended to output file names."
                ],
                "params" : [
                    "config",
                    "nodePath",
                    "fileList",
                    "jobPrefix"
                ],
                "returns" : []
            },
            {
                "desc" : "The maker function for publicly starting the job",
                "name" : "virtual void make()",
                "paramDesc" : [],
                "params" : [],
                "returns" : []
            },
            {
                "desc" : "Central function for event cuts",
                "name" : "virtual bool keepEvent()",
                "paramDesc" : [],
                "params" : [],
                "returns" : [
                    "true : Event should be processed",
                    "false : Event should be skipped"
                ]
            },
            {
                "desc" : "Called before the event loop for intialization",
                "name" : "virtual void preEventLoop()",
                "paramDesc" : [],
                "params" : [],
                "returns" : []
            },
            {
                "desc" : "Called after the event loop for cleanup, report generation, etc.",
                "name" : "virtual void postEventLoop()",
                "paramDesc" : [],
                "params" : [],
                "returns" : []
            },
            {
                "desc" : "Analyzes a single event in the chain",
                "name" : "virtual void analyzeEvent()",
                "paramDesc" : [],
                "params" : [],
                "returns" : []
            }
        ],
        "properties" : [
            {
                "desc" : "The logging object for the job",
                "name" : "Logger *logger"
            },
            {
                "desc" : "Project config",
                "name" : "XmlConfig *cfg"
            },
            {
                "desc" : "Node path in config file",
                "name" : "string nodePath"
            },
            {
                "desc" : "Store project histograms and data",
                "name" : "HistoBook *book"
            },
            {
                "desc" : "For generating generic reports",
                "name" : "Reporter *reporter"
            },
            {
                "desc" : "The chain object which gets automatically set up",
                "name" : "TChain *chain"
            },
            {
                "desc" : "",
                "name" : "int nEventsToProcess"
            }
        ]
    },
    "XmlConfig" : {
        "className" : "XmlConfig",
        "desc" : "Class for using xml files as project config files. See github for example config files <xmp>```<config></config></xmp>",
        "methods" : [
            {
                "desc" : "",
                "name" : "string getString( string nodePath, string def = \"\", bool trimW = true )",
                "paramDesc" : [
                    "Path to node",
                    "Default value if node DNE",
                    "Trim whitespace"
                ],
                "params" : [
                    "nodePath",
                    "default",
                    "trimW"
                ],
                "returns" : [
                    "String in config file or default"
                ]
            }
        ],
        "properties" : []
    }
}