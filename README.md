libniven
========

Niven is a C++14 library that simplifies the task of creating a REST-based API.

It is inspired by the C# [Nancy](http://nancyfx.org/) framework and uses
similar techniques for declaring routes and responses.

Niven is designed to be self-hosted and is built upon the
[libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) C library.

```cpp
class SimpleModule : public Module
{
	public:

		SimpleModule()
		{
			Get["/"] = [](auto) {
				return "The moon's a balloon";
			};
		}
};

REGISTER_MODULE(SimpleModule)
```

Features
--------

### Routes

Routes can contain literal segments as well as named captures.

```cpp
Get["/person/{id}"] = [](auto &c) {
	return "Someone with an ID of " + c["id"];
};
```


### Responses

A number of response types are supported simply by returning the appropriate value.
As seen above you can return a string which will result in a "text/plain" content
type and a status of ```Http::Ok```. Other responses include:

* HTTP status code, e.g. ```return Http::Unauthorized;```
* Path which will return a file on disk if it exists,
e.g. ```return Path(filename);```
* Entity or tree which will be automatically serialised to JSON,
e.g. ```return tree {{ "name", "Test" }};```

Additionally, by explicitly returning a Response object you can manipulate the
headers and cookies using a fluent syntax.

```cpp
Get["/"] = [](auto) {
	return Response()
		.WithHeader("Content-Type", "application/json")
		.WithCookie({ "niven-test", "chocolate chip", DateTime::Now().AddSeconds(30) });
};
```


### Dependencies

Niven contains a basic dependency container implementation.

Register a singleton with the host by passing in a shared_ptr:

```cpp
host.Register<MyType>(make_shared<MyDerivedType>());
```

Register a concrete class type with the host to be constructed
each time it is resolved with the given parameters:

```cpp
host.Register<MyType, MyDerivedType>(param1, param2...);
```

Within a request handler the dependency can be resolved via the context:
```cpp
Get["/{key}"] = [](Context &c) {
	return c.Resolve<MyType>()->Get(c["key"]);
};
```


### Entities and JSON

Niven uses [libentity](https://github.com/emergent-design/libentity) for dealing
with request/response JSON.

If you return either an object derived from ```ent::entity``` or an ```ent::tree```
as a response it will be automatically serialised as a JSON string and the response
content type set to "application/json".

Incoming JSON can be bound to an object, for example

```cpp
Post["/bind"] = [](Context &c) {
	return "The name is: " + c.Bind<MyEntity>().name;
};
```

### Further examples

Please take a look in the ```src/examples``` directory to see how to set
up the host and in ```include/examples``` for some simple commented modules.



Requirements
------------

* [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) -
the HTTP host back-end.
* [libgcrypt](http://www.gnu.org/software/libgcrypt/) -
for hashing and SSL.
* [libemergent](https://github.com/emergent-design/libemergent) -
type registration, file paths, datetime helper, logging, string helpers.
* [libentity](https://github.com/emergent-design/libentity) -
entity serialisation/deserialisation, trees.

A modern compiler that supports C++14 such as [clang](http://clang.llvm.org/)
and [premake v5.0](https://premake.github.io/).


Building on Ubuntu
------------------

```bash
$ sudo apt-get install libmicrohttpd-dev libgcrypt20-dev
```

Ensure that the headers for libemergent and libentity can be found on the standard
paths (for example ```/usr/local/include```).

```bash
$ premake5 gmake
$ make
```

To build the examples:
```bash
$ cd src/examples
$ premake5 gmake
$ make
```

When building an application with niven you must link against libniven and pthread.
