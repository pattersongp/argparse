# ArgParse

A simple argument parser for command line programs.

## Usage:

```cpp
ArgParse args();
args.addArg("-c", "--compile", [](){
		std::cout << "-c flag called!" << std::endl;
});
args.addArg("-g", "--good-doge", [&](){
		for(auto v : args.getActuals("-g")) {
			std::cout << v << std::endl;
		}
});
args.addAction([&](){
		for(auto v : args.getActuals("_imp")) {
			std::cout << v << std::endl;
		}
});
args.parse(argv);
```
