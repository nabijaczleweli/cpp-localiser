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
    /// Exceptions: strong guarantee.
    ///
    /// Returns: *this.
    localiser & merge(const localiser & loc);
    /// Exchange the contents of *this and with.
    ///
    /// Exceptions: none.
    ///
    /// Returns: *this.
    localiser & swap(localiser & with) noexcept;

    /// Check if no language mappings are loaded.
    ///
    /// Exceptions: none
    ///
    /// Returns:
    ///   * true, if no language mappings have been loaded.
    ///   * false, if any language mappings have been loaded.
    bool empty() const noexcept;
    /// Check if a particular language mapping exists.
    ///
    /// Exceptions: none
    ///
    /// Returns:
    ///   * true, if the supplied key corresponds to a language mapping.
    ///   * false, if any the supplied key does not correspond to any language mapping loaded.
    bool can_translate_key(const std::string & key) const noexcept;

    /// Translate a key according to the currently loaded language mappings.
    ///
    /// Exceptions: none
    ///
    /// Returns:
    ///   * The supplied key, if the language mapping does not exist (can_translate_key(key) == false).
    ///   * The language mapping's value, if the language mapping exists (can_translate_key(key) == true).
    const std::string & translate_key(const std::string & key) const noexcept;
  };
}

namespace std {
  /// Exchange the contents of lhs and rhs.
  ///
  /// Exceptions: none.
  void swap(cpp_localiser::localiser & lhs, cpp_localiser::localiser & rhs) noexcept;
}
```

## Data format

```
# Comment: Solaire's a lit lad, fam
key.name=Jolly co-valluation

second key name=Praise the sun!
utf-8=Powinno działać
```
