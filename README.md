# ProtonPreMix package

This module implements pre-mixing at RecHit level for protons.

In the test folder there are 3 example configs to run:
> - **RecHitInspector:** prints out the RecHits in an event.
> - **PreMixProducer:** module to perform the pre-mixing and set its variables.
> - **DirectSimFullChain:** example config for the full PPS simulation chain, from genParticle level protons, to reconstructed protons, with pre-mixing.

In order to run the full chain you need a file providing the crossing angle distribution for the period to which you are referring.