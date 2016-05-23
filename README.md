# cpp-localiser [![Licence](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE) [![TravisCI Build Status](https://travis-ci.org/nabijaczleweli/cpp-localiser.svg?branch=master)](https://travis-ci.org/nabijaczleweli/cpp-localiser) [![AppVeyorCI Build Status](https://ci.appveyor.com/api/projects/status/gnah19tp7b5ri60k?svg=true)](https://ci.appveyor.com/project/nabijaczleweli/cpp-localiser)
Localisation library for C++, not entirely unlike the Forge one


## API ref
```cpp
namespace cpp_localiser {
  class localiser {
  public:
    /// Construct an empty localiser (i.e. one without any mappings).
    ///
    /// Postconditions:
    ///   * empty() returns true.
    ///   * can_translate_key() always returns false.
    ///   * translate_key() always returns the key.
    localiser();
    /// Load localisation data from a stream.
    ///
    /// Refer to README for localisation data format.
    ///
    /// Postconditions:
    ///   * if the localisation data contains any keys:
    ///     - empty() returns false.
    ///   * otherwise:
    ///     - empty() returns true.
    ///     - can_translate_key() always returns false.
    ///     - translate_key() always returns the key.
    explicit localiser(std::istream & from);
    /// Equivalent to localiser(std::ifstream(localisation_root + '/' + locale + ".lang"))
    explicit localiser(const std::string & localisation_root, const std::string & locale = "en_US");
    /// Equivalent to localiser(loc0).merge(loc1)
    explicit localiser(const localiser & loc0, const localiser & loc1);

    /// Add all previously unmapped keys from loc to *this.
    ///
    /// Postconditions:
    ///   * For all X: loc.can_translate_key(X) => this->can_translate_key(X).
    ///
    /// Returns: *this.
    localiser & merge(const localiser & loc);
    /// Equivalent to *this = localiser(localisation_root, locale).
    ///
    /// Returns: *this.
    localiser & open(const std::string & localisation_root, const std::string & locale = "en_US");
    /// Exchange the contets of *this and with
    ///
    /// Returns: *this.
    localiser & swap(localiser & with);

    /// Check if no language mappings are loaded.
    ///
    /// Returns:
    ///   * true, if no language mappings have been loaded.
    ///   * false, if any language mappings have been loaded.
    bool empty() const;
    /// Check if a particular language mapping exists.
    ///
    /// Returns:
    ///   * true, if the supplied key corresponds to a language mapping.
    ///   * false, if any the supplied key does not correspond to any language mapping loaded.
    bool can_translate_key(const std::string & key) const;

    /// Translate a key according to the currently loaded language mappings.
    ///
    /// Returns:
    ///   * The supplied key, if the language mapping does not exist (can_translate_key(key) == false).
    ///   * The language mapping's value, if the language mapping exists (can_translate_key(key) == true).
    const std::string & translate_key(const std::string & key) const;
  };
}

namespace std {
  /// Equivalent to lhs.swap(rhs).
  void swap(cpp_localiser::localiser & lhs, cpp_localiser::localiser & rhs);
}
```

## Data format

```
# Comment: Solaire's a lit lad, fam
key.name=Jolly co-valluation

second key name=Praise the sun!
utf-8=Powinno działać
```
