from twttr import shorten

def test_nonstring():
    assert shorten("123") == "123"
    assert shorten(",,,") == ",,,"
    assert shorten(".") == "."
    assert shorten("") == ""
    assert shorten(" ") == " "

def test_string():
    assert shorten("Hello") == "Hll"
    assert shorten("dog") == "dg"
    assert shorten("twttr") == "twttr"
    assert shorten("10Leo") == "10L"
    assert shorten("HELLO") == "HLL"
    assert shorten("Hello, world") == "Hll, wrld"
