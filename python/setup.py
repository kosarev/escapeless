#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import setuptools

setuptools.setup(
    name='escapeless',
    version='1.0a0',
    packages=[],
    scripts=['escapeless.py'],
    install_requires=['future'],
    description='Efficient binary encoding for large alphabets.',
    author='Ivan Kosarev',
    author_email='ivan@kosarev.info',
    license='MIT',
    keywords='base64 enc binary encoding',
    url='https://github.com/kosarev/escapeless',
    # entry_points={
    #     'console_scripts': [
    #         'escapeless = escapeless:main',
    #     ],
    # },
    test_suite='tests',
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'Intended Audience :: Telecommunications Industry',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: Software Development',
    ],
)
